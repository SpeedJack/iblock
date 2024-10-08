#include "BlockchainManager.h"
#include "Wallet.h"
#include "iblock/bitcoin/messages/DirectBlockMsg.h"

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

		chainHistory = par("chainHistory").intValue();

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
	std::shared_ptr<const Block> block = check_and_cast<DirectBlockMsg*>(msg)->getBlockSharedPtr();
	// EV_INFO << "Received a new block (" << block->getTxnCount() << " txns)" << endl;
	appendBlock(block);
	delete msg;
}

std::shared_ptr<const Block> BlockchainManager::getCurrentBlock() const
{
	return mainBranch;
}

const BlockHeader* BlockchainManager::getCurrentBlockHeader() const
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

std::shared_ptr<const Block> BlockchainManager::findForkBlock(std::shared_ptr<const Block> a, std::shared_ptr<const Block> b) const
{
	if (a == b)
		return a;
	if (a->getHeight() > b->getHeight())
		return findForkBlock(a->getPrevBlock(), b);
	if (a->getHeight() < b->getHeight())
		return findForkBlock(a, b->getPrevBlock());
	return findForkBlock(a->getPrevBlock(), b->getPrevBlock());
}

void BlockchainManager::unconfirmWalletUtxos(std::shared_ptr<const Block> block) const
{
	for (Wallet* wallet : wallets) {
		const std::unordered_set<std::shared_ptr<const TransactionOutput>>* utxos = block->getUtxos(wallet);
		if (!utxos)
			continue;
		for (std::shared_ptr<const TransactionOutput> utxo : *utxos)
			wallet->unconfirmUtxo(utxo);
	}
}

void BlockchainManager::confirmWalletUtxos(std::shared_ptr<const Block> block) const
{
	for (Wallet* wallet : wallets) {
		const std::unordered_set<std::shared_ptr<const TransactionOutput>>* utxos = block->getUtxos(wallet);
		if (!utxos)
			continue;
		for (std::shared_ptr<const TransactionOutput> utxo : *utxos)
			wallet->confirmUtxo(utxo);
	}
}

void BlockchainManager::cleanup(uint32_t history)
{
	uint32_t height = mainBranch->getHeight();
	if (height <= history)
		return;

	for (auto it = branches.begin(); it != branches.end();) {
		std::shared_ptr<const Block> b = *it;
		if (b->getHeight() < height - history)
			it = branches.erase(it);
		else
			++it;
	}

	for (auto it = orphans.begin(); it != orphans.end();)
		if ((*it)->getHeight() < height - history)
			it = orphans.erase(it);
		else
			++it;

	std::shared_ptr<const Block> block = mainBranch;
	for (uint32_t curHeight = height; curHeight >= height - history && block; curHeight--)
		block = block->getPrevBlock();
	if (block)
		const_cast<BlockHeader*>(block->getHeader())->deletePrevBlock();
}

void BlockchainManager::appendBlock(std::shared_ptr<const Block> block)
{
	std::shared_ptr<const Block> prevBlock = block->getPrevBlock();
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
			mempoolManager->removeBlockTransactions(mainBranch);
			confirmWalletUtxos(mainBranch);
			onMainBranchAppend();
			//relay
			break;
		case 0x02: // block extends a side branch but does not make it the new main branch
			onSideBranchAppend(block);
			break;
		[[unlikely]] case 0x03: // block extends a side branch and makes it the new main branch
			std::shared_ptr<const Block> oldMainBranch = mainBranch;
			std::shared_ptr<const Block> forkBlock = findForkBlock(block, mainBranch);
			for (std::shared_ptr<const Block> b = mainBranch; b != forkBlock; b = b->getPrevBlock()) {
				unconfirmWalletUtxos(b);
				mempoolManager->addBlockTransactions(b);
			}
			mainBranch = forkBlock;
			std::vector<std::shared_ptr<const Block>> toAdd;
			for (std::shared_ptr<const Block> b = block; b != forkBlock; b = b->getPrevBlock())
				toAdd.push_back(b);
			while (!toAdd.empty()) {
				std::shared_ptr<const Block> b = toAdd.back();
				mainBranch = b;
				mempoolManager->removeBlockTransactions(b);
				confirmWalletUtxos(b);
				toAdd.pop_back();
			}
			mainBranch = block;
			onNewMainBranch(oldMainBranch, forkBlock);
			//relay
			break;
		}

		for (const auto& orphan : orphans)
			if (orphan->getPrevBlock() == block) {
				auto o = orphans.extract(orphan);
				onRemoveOrphan(o.value());
				appendBlock(o.value());
				return;
			}
		return;
	}

	if (height == 0)
		throw cRuntimeError("Genesis block already exists");

	for (const auto& branch : branches)
		for (std::shared_ptr<const Block> cur = branch; cur->getHeight() >= height; cur = cur->getPrevBlock()) {
			if (cur == block) {
				onDuplicateBlock(block);
				return; // duplicate block
			}
			if (cur->getPrevBlock() == block->getPrevBlock()) { // block creates a new branch
				branches.insert(block);
				onNewSideBranch(block);
				return;
			}
		}

	orphans.insert(block);
	onNewOrphan(block);

}

void BlockchainManager::onAfterMinedBlockAppend(std::shared_ptr<Block> block)
{
	for (auto node : nodeManager->nodes()) {
		if (node->getId() == this->getParentModule()->getId())
			continue;
		cGate* nodeGate = node->gate("blockchainManagerIn");
		// sendDirect(new BlockPl(block), nodeGate);
		sendDirect(new DirectBlockMsg(block), exponential(0.42), block->getBitLength() / ((double)1000*1000), nodeGate);
	}
}

void BlockchainManager::addBlock(std::shared_ptr<Block> block)
{
	Enter_Method("addBlock()");

	onBeforeMinedBlockAppend(block);

	appendBlock(std::const_pointer_cast<const Block>(block));

	onAfterMinedBlockAppend(block);
}

}
}
