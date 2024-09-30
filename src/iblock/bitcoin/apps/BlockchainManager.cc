#include "BlockchainManager.h"
#include "Wallet.h"

using namespace omnetpp;
using namespace iblock::bitcoin::payloads;

namespace iblock
{
namespace bitcoin
{

Define_Module(BlockchainManager)

void BlockchainManager::initialize(int stage)
{
	switch (stage) {
	case 0:
		AppBase::initialize(0);

		nodeManager = check_and_cast<NodeManager* >(getModuleByPath(par("nodeManagerModule").stringValue()));
		gbm = check_and_cast<GBM*>(getModuleByPath(par("gbmModule").stringValue()));

		mempoolManager = check_and_cast<MempoolManager* >(getModuleByPath(par("mempoolManagerModule").stringValue()));

		branches.clear();
		orphans.clear();

		dispatcher->registerListener(this, MessageKind::BLOCK);
		dispatcher->registerListener(this, MessageKind::GETDATA);
		dispatcher->registerListener(this, MessageKind::GETBLOCKS);
		dispatcher->registerListener(this, MessageKind::GETHEADERS);
		break;
	case 1: [[fallthrough]];
	case 2:
		AppBase::initialize(stage);
		break;
	case 3:
		AppBase::initialize(3);
	}
}

void BlockchainManager::handleBlockPacket(Peer* peer, BlockPl* block)
{
}

void BlockchainManager::handleGetDataPacket(Peer* peer, GetDataPl* getdata)
{
}

void BlockchainManager::handleGetBlocksPacket(Peer* peer, GetBlocksPl* getblocks)
{
	delete getblocks;
}

void BlockchainManager::handleGetHeadersPacket(Peer* peer, GetHeadersPl* getheaders)
{
	delete getheaders;
}

void BlockchainManager::handleOtherMessage(cMessage* msg)
{
	const Block* block = check_and_cast<BlockPl*>(msg)->getBlock();
	// EV_INFO << "Received a new block (" << block->getTxnCount() << " txns)" << endl;
	appendBlock(block);
	delete msg;
}

const Block *BlockchainManager::getCurrentBlock() const
{
	return mainBranch;
}

const BlockHeader *BlockchainManager::getCurrentBlockHeader() const
{
	return this->getCurrentBlock()->getHeader();
}

Hash BlockchainManager::getCurrentTargetNBits() const
{
	return this->getCurrentBlockHeader()->getNBits();
}

uint32_t BlockchainManager::getCurrentHeight() const
{
	return this->getCurrentBlock()->getHeight();
}

Hash BlockchainManager::getNextTargetNBits() const
{
	return this->getCurrentTargetNBits(); // TODO
}

const Block* BlockchainManager::findForkBlock(const Block* a, const Block* b) const
{
	if (a == b)
		return a;
	if (a->getHeight() > b->getHeight())
		return findForkBlock(a->getPrevBlock(), b);
	if (a->getHeight() < b->getHeight())
		return findForkBlock(a, b->getPrevBlock());
	return findForkBlock(a->getPrevBlock(), b->getPrevBlock());
}

void BlockchainManager::unconfirmWalletUtxos(const Block* block) const
{
	for (Wallet* wallet : wallets) {
		const std::unordered_set<const TransactionOutput*>* utxos = block->getUtxos(wallet);
		if (!utxos)
			continue;
		for (const TransactionOutput* utxo : *utxos)
			wallet->unconfirmUtxo(utxo);
	}
}

void BlockchainManager::confirmWalletUtxos(const Block* block) const
{
	for (Wallet* wallet : wallets) {
		const std::unordered_set<const TransactionOutput*>* utxos = block->getUtxos(wallet);
		if (!utxos)
			continue;
		for (const TransactionOutput* utxo : *utxos)
			wallet->confirmUtxo(utxo);
	}
}

void BlockchainManager::appendBlock(const Block* block)
{
	const Block* prevBlock = block->getPrevBlock();
	if (!prevBlock)
		throw cRuntimeError("Block has no previous block");
	const auto& it = branches.find(prevBlock);

	uint32_t height = block->getHeight();
	if (it != branches.end()) {
		branches.erase(it);
		branches.insert(block);
		unsigned char type = 0;
		if (height > mainBranch->getHeight())
			type |= 0x01;
		if (prevBlock != mainBranch)
			type |= 0x02;
		switch (type) {
		[[likely]] case 0x01: // block further extends the main branch
			mainBranch = block;
			mempoolManager->removeBlockTransactions(block);
			confirmWalletUtxos(block);
			//relay
			break;
		case 0x02: // block extends a side branch but does not make it the new main branch
			break;
		[[unlikely]] case 0x03: // block extends a side branch and makes it the new main branch
			const Block* forkBlock = findForkBlock(block, mainBranch);
			for (const Block* b = mainBranch; b != forkBlock; b = b->getPrevBlock()) {
				unconfirmWalletUtxos(b);
				mempoolManager->addBlockTransactions(b);
			}
			mainBranch = forkBlock;
			std::vector<const Block*> toAdd;
			for (const Block* b = block; b != forkBlock; b = b->getPrevBlock())
				toAdd.push_back(b);
			while (!toAdd.empty()) {
				const Block* b = toAdd.back();
				mainBranch = b;
				mempoolManager->removeBlockTransactions(b);
				confirmWalletUtxos(b);
				toAdd.pop_back();
			}
			mainBranch = block;
			//relay
			break;
		}

		for (const auto& orphan : orphans)
			if (orphan->getPrevBlock() == block) {
				auto o = orphans.extract(orphan);
				appendBlock(o.value());
				return;
			}
		return;
	}

	if (height == 0)
		throw cRuntimeError("Genesis block already exists");

	for (const auto& branch : branches)
		for (const Block* cur = branch; cur->getHeight() >= height; cur = cur->getPrevBlock()) {
			if (cur == block)
				return; // duplicate block
			if (cur->getPrevBlock() == block->getPrevBlock()) { // block creates a new branch
				branches.insert(block);
				return;
			}
		}

	orphans.insert(block);

}

void BlockchainManager::addBlock(Block* block)
{
	Enter_Method("addBlock()");

	gbm->addBlock(block);
	appendBlock(block);

	for (auto node : nodeManager->nodes()) {
		if (node->getId() == this->getParentModule()->getId())
			continue;
		cGate* nodeGate = node->gate("blockchainManagerIn");
		// sendDirect(new BlockPl(block), nodeGate);
		sendDirect(new BlockPl(block), exponential(0.42), block->getBitLength() / ((double)1000*1000), nodeGate);
	}
}

}
}
