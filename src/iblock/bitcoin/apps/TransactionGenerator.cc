#include "TransactionGenerator.h"
#include "iblock/bitcoin/objects/Transaction.h"

using namespace omnetpp;

Define_Module(iblock::bitcoin::TransactionGenerator);

namespace iblock
{
namespace bitcoin
{

void TransactionGenerator::initialize()
{
	AppBase::initialize();
	mempoolManager = check_and_cast<MempoolManager *>(getModuleByPath(par("mempoolManagerModule").stringValue()));
	createTransactionMsg = new cMessage("createTransactionMsg");
	scheduleAt(simTime() + par("interval").doubleValue(), createTransactionMsg);
}

void TransactionGenerator::handleSelfMessage(cMessage *msg)
{
	createTransaction();
	scheduleAt(simTime() + par("interval").doubleValue(), msg);
}

void TransactionGenerator::createTransaction()
{
	TransactionInput *input = new TransactionInput();
	TransactionOutput *output = new TransactionOutput(new BitcoinAddress(), satoshi(par("amount").doubleValue()));
	Transaction *tx = new Transaction();
	tx->setVersion(70015);
	tx->appendTxIn(input);
	tx->appendTxOut(output);

	mempoolManager->addTransaction(tx);
}

int64_t TransactionGenerator::satoshi(double amount) const
{
	if (amount < 0)
		return 0;
	return static_cast<int64_t>(std::ceil(amount * 100000000));
}

}
}
