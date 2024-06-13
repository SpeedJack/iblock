#ifndef __IBLOCK_BITCOIN_BLOCKCHAINMANAGER_H_
#define __IBLOCK_BITCOIN_BLOCKCHAINMANAGER_H_

#include "iblock/bitcoin/apps/base/AppBase.h"
#include "iblock/bitcoin/objects/Block.h"
#include "iblock/bitcoin/global/GBM.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API BlockchainManager : public AppBase
{
	protected:
		GBM *gbm;

		virtual void initialize() override;

		virtual void handleBlockPacket(Peer *peer, payloads::BlockPl *block) override;
		virtual void handleGetDataPacket(Peer *peer, payloads::GetDataPl *getdata) override;
		virtual void handleGetBlocksPacket(Peer *peer, payloads::GetBlocksPl *getblocks) override;
		virtual void handleGetHeadersPacket(Peer *peer, payloads::GetHeadersPl *getheaders) override;

	public:
		BlockchainManager() : AppBase() { gbm = nullptr; }

		virtual Block *getCurrentBlock() const;
		virtual uint32_t getCurrentHeight() const { return getCurrentBlock()->getHeight(); }
		virtual const BlockHeader *getCurrentBlockHeader() const { return getCurrentBlock()->getHeader(); }
		virtual Hash getCurrentTargetNBits() const { return getCurrentBlockHeader()->getNBits(); }
		virtual Hash getNextTargetNBits() const;
		virtual void addBlock(Block *block);
};

}
}

#endif
