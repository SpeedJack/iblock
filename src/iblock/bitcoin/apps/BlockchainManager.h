#ifndef __IBLOCK_BITCOIN_BLOCKCHAINMANAGER_H_
#define __IBLOCK_BITCOIN_BLOCKCHAINMANAGER_H_

#include "iblock/bitcoin/apps/base/AppBase.h"
#include "iblock/bitcoin/objects/Block.h"
#include "iblock/bitcoin/global/NodeManager.h"
#include "iblock/bitcoin/apps/MempoolManager.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API BlockchainManager : public AppBase
{
	protected:
		NodeManager* nodeManager;
		MempoolManager* mempoolManager;
		std::unordered_set<const Block*> branches;
		const Block* mainBranch;
		std::unordered_set<const Block*> orphans;
		std::vector<Wallet *> wallets = std::vector<Wallet *>();

		virtual void initialize(int stage) override;
		virtual int numInitStages() const override { return 4; }

		virtual void handleBlockPacket(Peer* peer, payloads::BlockPl* block) override;
		virtual void handleGetDataPacket(Peer* peer, payloads::GetDataPl* getdata) override;
		virtual void handleGetBlocksPacket(Peer* peer, payloads::GetBlocksPl* getblocks) override;
		virtual void handleGetHeadersPacket(Peer* peer, payloads::GetHeadersPl* getheaders) override;
		virtual void handleOtherMessage(::omnetpp::cMessage* msg) override;

		virtual void appendBlock(const Block* block);

		virtual void unconfirmWalletUtxos(const Block* block) const;
		virtual void confirmWalletUtxos(const Block* block) const;
		virtual const Block* findForkBlock(const Block* a, const Block* b) const;

	public:
		BlockchainManager() : AppBase() { nodeManager = nullptr; mempoolManager = nullptr; mainBranch = nullptr; }

		virtual const Block* getCurrentBlock() const;
		virtual uint32_t getCurrentHeight() const;
		virtual const BlockHeader* getCurrentBlockHeader() const;
		virtual Hash getCurrentTargetNBits() const;
		virtual Hash getNextTargetNBits() const;
		virtual void addBlock(Block* block);
		void registerWallet(Wallet* wallet) { Enter_Method_Silent("registerWallet()"); wallets.push_back(wallet); }
};

}
}

#endif
