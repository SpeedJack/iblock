#ifndef __IBLOCK_BITCOIN_BLOCKCHAINMANAGER_H_
#define __IBLOCK_BITCOIN_BLOCKCHAINMANAGER_H_

#include "iblock/bitcoin/apps/base/AppBase.h"
#include "iblock/bitcoin/objects/Block.h"
#include "iblock/bitcoin/global/NodeManager.h"
#include "iblock/bitcoin/global/GBM.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API BlockchainManager : public AppBase
{
	protected:
		//GBM *gbm;
		NodeManager *nodeManager;
		Block *currentBlock;

		virtual void initialize() override;

		virtual void handleBlockPacket(Peer *peer, payloads::BlockPl *block) override;
		virtual void handleGetDataPacket(Peer *peer, payloads::GetDataPl *getdata) override;
		virtual void handleGetBlocksPacket(Peer *peer, payloads::GetBlocksPl *getblocks) override;
		virtual void handleGetHeadersPacket(Peer *peer, payloads::GetHeadersPl *getheaders) override;
		virtual void handleOtherMessage(::omnetpp::cMessage *msg) override;

	public:
		//BlockchainManager() : AppBase() { gbm = nullptr; }
		BlockchainManager() : AppBase() { nodeManager = nullptr; currentBlock = nullptr; }

		virtual Block *getCurrentBlock() const;
		virtual uint32_t getCurrentHeight() const;
		virtual const BlockHeader *getCurrentBlockHeader() const;
		virtual Hash getCurrentTargetNBits() const;
		virtual Hash getNextTargetNBits() const;
		virtual void addBlock(Block *block);
};

}
}

#endif
