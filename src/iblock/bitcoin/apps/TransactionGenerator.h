#ifndef __IBLOCK_BITCOIN_TRANSACTIONGENERATOR_H_
#define __IBLOCK_BITCOIN_TRANSACTIONGENERATOR_H_

#include "MempoolManager.h"
#include "iblock/bitcoin/global/WalletManager.h"
#include "iblock/bitcoin/messages/CreateTransactionMessage_m.h"

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
		CreateTransactionMessage* createTransactionMsg;
		unsigned int minConfirmations;
		double waitTime;

		virtual void initialize() override;
		virtual void handleSelfMessage(::omnetpp::cMessage* msg) override;
		virtual void createTransaction(satoshi_t transactionValue);

	public:
		TransactionGenerator() : AppBase() { mempoolManager = nullptr; walletManager = nullptr; createTransactionMsg = nullptr; }
};

}
}

#endif
