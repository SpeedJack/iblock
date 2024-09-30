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
		::omnetpp::cArray* blocks;
		unsigned long long totalTx;
		Coinbase* coinbaseTx;

		virtual void initialize(int stage) override;
		virtual int numInitStages() const override { return 3; }
		virtual void handleMessage(::omnetpp::cMessage* msg) override { delete msg; }
		virtual Hash computeInitialNBits() const;
		virtual ~GBM() override { delete coinbaseTx; delete blocks; }

	public:
		GBM() : ::omnetpp::cSimpleModule() { blocks = nullptr; coinbaseTx = nullptr; }
		virtual void addBlock(Block* block);
};

}
}

#endif
