#ifndef __IBLOCK_BITCOIN_TRANSACTIONGENERATOR_H_
#define __IBLOCK_BITCOIN_TRANSACTIONGENERATOR_H_

#include "MempoolManager.h"
#include "iblock/bitcoin/global/WalletManager.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API TransactionGenerator : public AppBase
{
	protected:
		MempoolManager* mempoolManager;
		WalletManager* walletManager;
		Wallet* wallet;
		::omnetpp::cMessage* createTransactionMsg;
		unsigned int minConfirmations;
		double waitTime;
		::omnetpp::simsignal_t txInputValueSignal;
		::omnetpp::simsignal_t txOutputValueSignal;
		::omnetpp::simsignal_t txInputCountSignal;
		::omnetpp::simsignal_t txOutputCountSignal;
		::omnetpp::simsignal_t txFeeSignal;
		::omnetpp::simsignal_t txSizeSignal;

		virtual void initialize() override;
		virtual void handleSelfMessage(::omnetpp::cMessage* msg) override;
		virtual void createTransaction(satoshi_t transactionValue);

	public:
		TransactionGenerator() : AppBase() { mempoolManager = nullptr; walletManager = nullptr; createTransactionMsg = nullptr; }
		~TransactionGenerator() { if (createTransactionMsg && createTransactionMsg->isScheduled()) cancelAndDelete(createTransactionMsg); else if (createTransactionMsg) delete createTransactionMsg; }
};

}
}

#endif
