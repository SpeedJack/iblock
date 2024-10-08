#ifndef __IBLOCK_BITCOIN_BLOCKCHAINMANAGER_H_
#define __IBLOCK_BITCOIN_BLOCKCHAINMANAGER_H_

#include "iblock/bitcoin/apps/base/AppBase.h"
#include "iblock/bitcoin/global/GBM.h"
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
		GBM* gbm;
		MempoolManager* mempoolManager;
		std::unordered_set<std::shared_ptr<const Block>> branches;
		std::shared_ptr<const Block> mainBranch;
		std::unordered_set<std::shared_ptr<const Block>> orphans;
		std::vector<Wallet *> wallets = std::vector<Wallet *>();
		unsigned int chainHistory = 10;

		virtual void initialize(int stage) override;
		virtual int numInitStages() const override { return 4; }

		virtual void handleBlockPacket(Peer* peer, payloads::BlockPl* block) override;
		virtual void handleGetDataPacket(Peer* peer, payloads::GetDataPl* getdata) override;
		virtual void handleGetBlocksPacket(Peer* peer, payloads::GetBlocksPl* getblocks) override;
		virtual void handleGetHeadersPacket(Peer* peer, payloads::GetHeadersPl* getheaders) override;
		virtual void handleOtherMessage(::omnetpp::cMessage* msg) override;

		virtual void onMainBranchAppend() { cleanup(chainHistory); }
		virtual void onNewSideBranch(std::shared_ptr<const Block> branch) { }
		virtual void onSideBranchAppend(std::shared_ptr<const Block> newBlock) { }
		virtual void onNewMainBranch(std::shared_ptr<const Block> oldBranch, std::shared_ptr<const Block> forkBlock) { cleanup(chainHistory); }
		virtual void onNewOrphan(std::shared_ptr<const Block> orphan) { }
		virtual void onRemoveOrphan(std::shared_ptr<const Block> orphan) { }
		virtual void onDuplicateBlock(std::shared_ptr<const Block> block) { }
		virtual void onBeforeMinedBlockAppend(std::shared_ptr<Block> block) { }
		virtual void onAfterMinedBlockAppend(std::shared_ptr<Block> block);

		virtual void appendBlock(std::shared_ptr<const Block> block);

		virtual void unconfirmWalletUtxos(std::shared_ptr<const Block> block) const;
		virtual void confirmWalletUtxos(std::shared_ptr<const Block> block) const;
		virtual std::shared_ptr<const Block> findForkBlock(std::shared_ptr<const Block> a, std::shared_ptr<const Block> b) const;

		virtual void cleanup(uint32_t history = 10);

	public:
		BlockchainManager() : AppBase() { nodeManager = nullptr; gbm = nullptr; mempoolManager = nullptr; mainBranch = nullptr; }

		virtual std::shared_ptr<const Block> getCurrentBlock() const;
		virtual uint32_t getCurrentHeight() const;
		virtual const BlockHeader* getCurrentBlockHeader() const;
		virtual Hash getCurrentTargetNBits() const;
		virtual Hash getNextTargetNBits() const;
		virtual void addBlock(std::shared_ptr<Block> block);
		virtual void registerWallet(Wallet* wallet) { Enter_Method_Silent("registerWallet()"); wallets.push_back(wallet); }

		virtual void addGenesisBlock(std::shared_ptr<const Block> block) { Enter_Method_Silent("addGenesisBlock()"); if (mainBranch) throw std::runtime_error("Genesis block already set"); mainBranch = block; branches.insert(mainBranch); confirmWalletUtxos(block); }
};

}
}

#endif
