#include "BlockchainManager.h"

using namespace omnetpp;
using namespace iblock::bitcoin::payloads;

namespace iblock
{
namespace bitcoin
{

Define_Module(BlockchainManager)

void BlockchainManager::initialize()
{
	AppBase::initialize();

	//gbm = check_and_cast<GBM *>(getModuleByPath(par("globalBlockchainManagerModule").stringValue()));
	nodeManager = check_and_cast<NodeManager *>(getModuleByPath(par("nodeManagerModule").stringValue()));
	nodeManager->registerNode(this);

	dispatcher->registerListener(this, MessageKind::BLOCK);
	dispatcher->registerListener(this, MessageKind::GETDATA);
	dispatcher->registerListener(this, MessageKind::GETBLOCKS);
	dispatcher->registerListener(this, MessageKind::GETHEADERS);
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

void handleOtherMessage(cMessage *msg)
{
	EV << "Received a new block" << endl;
	delete msg;
}

Block *BlockchainManager::getCurrentBlock() const
{
	return nullptr;
}

void BlockchainManager::addBlock(Block *block)
{
	Enter_Method("addBlock()");
	//gbm->addBlock(block);
	std::vector<cModule *> nodes = nodeManager->getNodes();
	for (auto node : nodes) {
		if (node->getId() == this->getId())
			continue;
		cGate *nodeGate = node->gate("dIn");
		sendDirect(new BlockPl(block), SimTime(2, SIMTIME_S), SimTime(100, SIMTIME_MS), nodeGate);
	}
}

}
}
