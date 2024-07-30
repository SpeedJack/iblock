#ifndef __IBLOCK_BITCOIN_TRANSACTIONGENERATOR_H_
#define __IBLOCK_BITCOIN_TRANSACTIONGENERATOR_H_

#include "MempoolManager.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API TransactionGenerator : public AppBase
{
	protected:
		MempoolManager *mempoolManager;
		::omnetpp::cMessage *createTransactionMsg;

		virtual void initialize() override;
		virtual void handleSelfMessage(::omnetpp::cMessage *msg) override;
		virtual void createTransaction();
		int64_t satoshi(double amount) const;

	public:
		TransactionGenerator() : AppBase() { mempoolManager = nullptr; createTransactionMsg = nullptr; }
};

}
}

#endif
