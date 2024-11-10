#ifndef __IBLOCK_BITCOIN_GBM_H_
#define __IBLOCK_BITCOIN_GBM_H_

#include "iblock/bitcoin/objects/Block.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API GBM : public ::omnetpp::cSimpleModule
{
	protected:
		std::unordered_map<int, std::shared_ptr<const Block>> mainBranch;
		::omnetpp::cMessage* cleanupMsg;
		double cleanupInterval;

		virtual void initialize(int stage) override;
		virtual int numInitStages() const override { return 3; }
		virtual void handleMessage(::omnetpp::cMessage* msg) override;
		virtual Hash computeInitialNBits() const;
		virtual uint32_t deleteOldBlocks();
		virtual std::shared_ptr<const Block> findForkBlock(std::shared_ptr<const Block> a, std::shared_ptr<const Block> b) const;

	public:
		GBM() : ::omnetpp::cSimpleModule() { }
		~GBM();

		virtual void adviseNewMainBranch(int moduleId, std::shared_ptr<const Block> newMainBranch) { Enter_Method_Silent("adviseNewMainBranch()"); mainBranch[moduleId] = newMainBranch; }
};

}
}

#endif
