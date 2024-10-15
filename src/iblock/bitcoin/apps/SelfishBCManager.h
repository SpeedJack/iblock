#ifndef __IBLOCK_BITCOIN_SELFISHBCMANAGER_H_
#define __IBLOCK_BITCOIN_SELFISHBCMANAGER_H_

#include <deque>
#include "iblock/bitcoin/apps/BlockchainManager.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API SelfishBCManager : public BlockchainManager
{
	protected:
		unsigned int selfishChainDistance = 0;
		std::deque<std::shared_ptr<Block>> selfishChain;
		unsigned int maxSelfishChainLength = 0;
		::omnetpp::simsignal_t selfishChainLengthSignal;
		::omnetpp::simsignal_t selfishChainDistanceSignal;
		::omnetpp::simsignal_t attackResultSignal;

		virtual void initialize(int stage) override;

		virtual unsigned int computeDistance(std::shared_ptr<const Block> first, std::shared_ptr<const Block> second);

		virtual void relaySelfishChain(std::shared_ptr<const Block> startBlock);
		virtual void relaySelfishChainIfNeeded(std::shared_ptr<const Block> receivedBlock);

		virtual void onNewSideBranch(std::shared_ptr<const Block> branch) override;
		virtual void onSideBranchAppend(std::shared_ptr<const Block> newBlock) override;
		virtual void onNewMainBranch(std::shared_ptr<const Block> oldBranch, std::shared_ptr<const Block> forkBlock) override;
		virtual void onBeforeMinedBlockAppend(std::shared_ptr<Block> block) override;
		virtual void onAfterMinedBlockAppend(std::shared_ptr<Block> block) override;

	public:
		SelfishBCManager() : BlockchainManager() { }
};

}
}

#endif
