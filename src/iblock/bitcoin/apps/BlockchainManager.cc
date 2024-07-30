#include "BlockchainManager.h"

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

		nodeManager = check_and_cast<NodeManager *>(getModuleByPath(par("nodeManagerModule").stringValue()));
		nodeManager->registerBlockchainManager(this);

		mempoolManager = check_and_cast<MempoolManager *>(getModuleByPath(par("mempoolManagerModule").stringValue()));

		branches.clear();
		orphans.clear();

		dispatcher->registerListener(this, MessageKind::BLOCK);
		dispatcher->registerListener(this, MessageKind::GETDATA);
		dispatcher->registerListener(this, MessageKind::GETBLOCKS);
		dispatcher->registerListener(this, MessageKind::GETHEADERS);
		break;
	case 1:
		AppBase::initialize(1);

		mainBranch = nodeManager->getGenesisBlock();
		branches.insert(mainBranch);
	}
}

void BlockchainManager::handleBlockPacket(Peer *peer, BlockPl *block)
{
}

void BlockchainManager::handleGetDataPacket(Peer *peer, GetDataPl *getdata)
{
}

void BlockchainManager::handleGetBlocksPacket(Peer *peer, GetBlocksPl *getblocks)
{
	delete getblocks;
}

void BlockchainManager::handleGetHeadersPacket(Peer *peer, GetHeadersPl *getheaders)
{
	delete getheaders;
}

void BlockchainManager::handleOtherMessage(cMessage *msg)
{
	const Block *block = check_and_cast<BlockPl *>(msg)->getBlock();
	EV << "Received a new block (" << block->getTxnCount() << " txns)" << endl;
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
	return this->getCurrentTargetNBits();
}

void BlockchainManager::appendBlock(const Block *block)
{
	const Block *prevBlock = block->getPrevBlock();
	const auto &it = branches.find(prevBlock);

	uint32_t height = block->getHeight();
	if (it != branches.end()) {
		branches.erase(it);
		branches.insert(block);
		if (height > mainBranch->getHeight()) {
			mainBranch = block;
			mempoolManager->removeTransactions(block);
		} // TODO: if switching to side branch, re-add transactions to mempool

		for (const auto &orphan : orphans)
			if (orphan->getPrevBlock() == block) {
				auto o = orphans.extract(orphan);
				appendBlock(o.value());
				return;
			}
		return;
	}

	if (height == 0)
		throw cRuntimeError("Genesis block already exists");

	for (const auto &branch : branches)
		for (const Block *cur = branch; height <= cur->getHeight(); cur = cur->getPrevBlock())
			if (cur == block)
				return; // duplicate block

	orphans.insert(block);

}

void BlockchainManager::addBlock(Block *block)
{
	Enter_Method("addBlock()");

	const Block *prevBlock = block->getPrevBlock();
	if (mainBranch != prevBlock)
		throw cRuntimeError("Mined an invalid block");
	branches.erase(prevBlock);
	branches.insert(block);
	mainBranch = block;

	mempoolManager->removeTransactions(block);

	std::vector<BlockchainManager *> nodes = nodeManager->getBlockchainManagers();
	for (auto node : nodes) {
		if (node->getId() == this->getId())
			continue;
		cGate *nodeGate = node->gate("dIn");
		sendDirect(new BlockPl(block), nodeGate);
	}
}

}
}
