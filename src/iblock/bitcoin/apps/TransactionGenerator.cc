#include "TransactionGenerator.h"
#include "Wallet.h"
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
	mempoolManager = check_and_cast<MempoolManager*>(getModuleByPath(par("mempoolManagerModule").stringValue()));
	nodeManager = check_and_cast<NodeManager* >(getModuleByPath(par("nodeManagerModule").stringValue()));
	wallet = check_and_cast<Wallet*>(getModuleByPath(par("walletModule").stringValue()));
	createTransactionMsg = new cMessage("createTransactionMsg");
	scheduleAt(simTime() + par("interval").doubleValue(), createTransactionMsg);
}

void TransactionGenerator::handleSelfMessage(cMessage *msg)
{
	createTransaction();
	double waitTime = wallet->balance() == 0 ? 5*60 : par("interval").doubleValue();
	scheduleAt(simTime() + waitTime, msg);
}

void TransactionGenerator::createTransaction()
{
	if (wallet->balance() == 0)
		return;
	Transaction* tx = new Transaction();
	tx->setVersion(70015);
	double transactionValueBtc = par("amount").doubleValue();
	int64_t transactionValue = SATOSHIS(transactionValueBtc);
	if (transactionValue > wallet->balance() - 1)
		transactionValue = wallet->balance() - 1;
	int64_t foundValue = 0;
	std::vector<const TransactionOutput*> utxos = wallet->unspentOutputs(transactionValue + 1);
	tx->setTxInArraySize(utxos.size());
	size_t index = 0;
	for (const TransactionOutput* utxo : utxos) {
		TransactionInput* input = new TransactionInput();
		input->setPrevOutput(const_cast<TransactionOutput*>(utxo));
		tx->setTxIn(index++, input);
		foundValue += input->getValue();
	}
	tx->setTxOutArraySize(2);
	TransactionOutput* output = new TransactionOutput();
	output->setAddress(nodeManager->getRandomBitcoinAddress());
	tx->setTxOut(0, output);
	TransactionOutput* change = new TransactionOutput();
	change->setAddress(wallet->getNewAddress());
	tx->setTxOut(1, change);

	uint32_t bytes = tx->getByteLength();
	int64_t fee = par("feeRate").intValue() * bytes;
	if (fee > (transactionValue - 2))
		fee = transactionValue - 2;
	output->setValue(transactionValue - fee);
	change->setValue(foundValue - transactionValue);

	mempoolManager->addTransaction(tx);
}

}
}
