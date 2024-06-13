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

	gbm = check_and_cast<GBM *>(getModuleByPath(par("globalBlockchainManagerModule").stringValue()));

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

Block *BlockchainManager::getCurrentBlock() const
{
	
}

void BlockchainManager::addBlock(Block *block)
{
	Enter_Method("addBlock()");
	gbm->addBlock(block);
}

}
}
