#ifndef __IBLOCK_BITCOIN_GMM_H_
#define __IBLOCK_BITCOIN_GMM_H_

#include "iblock/bitcoin/objects/Transaction.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API GMM : public ::omnetpp::cSimpleModule
{
	protected:
		::omnetpp::cArray* transactions;

		virtual void initialize() override { transactions = new ::omnetpp::cArray("transactions", 1024, 1024); transactions->setTakeOwnership(true); }
		virtual void handleMessage(::omnetpp::cMessage* msg) override { delete msg; }
		virtual ~GMM() override { delete transactions; }

	public:
		GMM() : ::omnetpp::cSimpleModule() { transactions = nullptr; }
		virtual void addTransaction(Transaction* tx) { Enter_Method_Silent("addBlock()"); transactions->add(tx); }
};

}
}

#endif
