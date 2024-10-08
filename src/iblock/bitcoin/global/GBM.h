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
		std::vector<std::shared_ptr<Block>> blocks;
		unsigned long long totalTx;
		std::shared_ptr<Coinbase> coinbaseTx;

		virtual void initialize(int stage) override;
		virtual int numInitStages() const override { return 3; }
		virtual void handleMessage(::omnetpp::cMessage* msg) override { delete msg; }
		virtual Hash computeInitialNBits() const;

	public:
		GBM() : ::omnetpp::cSimpleModule() { coinbaseTx = nullptr; }
		virtual void addBlock(std::shared_ptr<Block> block);
};

}
}

#endif
