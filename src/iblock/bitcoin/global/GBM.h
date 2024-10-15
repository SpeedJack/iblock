#ifndef __IBLOCK_BITCOIN_GBM_H_
#define __IBLOCK_BITCOIN_GBM_H_

#include "iblock/bitcoin/objects/Coinbase.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API GBM : public ::omnetpp::cSimpleModule
{
	protected:
		std::shared_ptr<Coinbase> coinbaseTx;

		virtual void initialize(int stage) override;
		virtual int numInitStages() const override { return 3; }
		virtual void handleMessage(::omnetpp::cMessage* msg) override { delete msg; }
		virtual Hash computeInitialNBits() const;

	public:
		GBM() : ::omnetpp::cSimpleModule() { coinbaseTx = nullptr; }
};

}
}

#endif
