#include "TransactionGenerator.h"
#include "Wallet.h"
#include "iblock/bitcoin/objects/Transaction.h"
#include <numeric>

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
	walletManager = check_and_cast<WalletManager*>(getModuleByPath(par("walletManagerModule").stringValue()));
	wallet = check_and_cast<Wallet*>(getModuleByPath(par("walletModule").stringValue()));
	createTransactionMsg = new CreateTransactionMessage("createTransactionMsg");
	minConfirmations = par("minConfirmations").intValue();
	waitTime = par("waitTime").doubleValue();
	satoshi_t transactionValue = par("amount");
	createTransactionMsg->setAmount(transactionValue);
	scheduleAt(simTime() + par("interval").doubleValue(), createTransactionMsg);
}

void TransactionGenerator::handleSelfMessage(cMessage *msg)
{
	satoshi_t transactionValue = createTransactionMsg->getAmount();
	if (wallet->mayHaveBalance(transactionValue, minConfirmations)) {
		createTransaction(transactionValue);
		createTransactionMsg->setAmount(par("amount"));
		scheduleAt(simTime() + par("interval").doubleValue(), createTransactionMsg);
	} else if (waitTime > 0.0) {
		scheduleAt(simTime() + waitTime, msg);
	} else {
		scheduleAt(simTime() + par("interval").doubleValue(), msg);
	}
}

void TransactionGenerator::createTransaction(satoshi_t transactionValue)
{
	satoshi_t foundValue = 0;
	std::vector<const TransactionOutput*> allUtxos = wallet->unspentOutputs(minConfirmations);
	std::vector<const TransactionOutput*> utxos;
	std::vector<const TransactionOutput*>::const_iterator it = allUtxos.begin();
	while (foundValue < transactionValue && it != allUtxos.end()) {
		const TransactionOutput* utxo = *it;
		utxos.push_back(utxo);
		foundValue += utxo->getValue();
		++it;
	}
	if (foundValue < transactionValue)
		return;

	Transaction* tx = new Transaction();
	tx->setVersion(70015);

	tx->setTxInArraySize(utxos.size());
	size_t index = 0;
	for (const TransactionOutput* utxo : utxos) {
		TransactionInput* input = new TransactionInput();
		input->setPrevOutput(const_cast<TransactionOutput*>(utxo));
		tx->setTxIn(index++, input);
	}

	int destCount = par("destCount").intValue();
	std::vector<double> splits;
	splits.reserve(destCount);
	for (int i = 0; i < destCount; ++i)
		splits.push_back(par("outputSplitDistrib").doubleValue());
	double splitSum = std::accumulate(splits.begin(), splits.end(), 0.0);
	std::for_each(splits.begin(), splits.end(), [splitSum](double& split) { split /= splitSum; });

	satoshi_t usedValue = 0;
	tx->setTxOutArraySize(destCount);
	index = 0;
	for (int i = 0; i < destCount; ++i) {
		TransactionOutput* output = new TransactionOutput();
		output->setAddress(walletManager->getRandomBitcoinAddress());
		satoshi_t curValue = transactionValue * splits[i];
		output->setValue(curValue);
		usedValue += curValue;
		tx->setTxOut(index++, output);
	}

	TransactionOutput* change = nullptr;
	while (true) {
		long long remainingValue = foundValue.sat() - usedValue.sat();
		double feeRate = par("feeRate").doubleValue();
		satoshi_t fee = feeRate * tx->getByteLength();
		if (fee < remainingValue) {
			if (!change) {
				change = new TransactionOutput();
				change->setAddress(wallet->getNewAddress());
				change->setValue(remainingValue - fee.sat());
				tx->appendTxOut(change);
				continue;
			}
			change->setValue(remainingValue - fee.sat());
			break;
		}
		if (fee > remainingValue) {
			if (it == allUtxos.end()) {
				delete tx;
				return;
			}			
			TransactionOutput* utxo = const_cast<TransactionOutput*>(*it);
			TransactionInput* input = new TransactionInput();
			input->setPrevOutput(utxo);
			tx->appendTxIn(input);
			foundValue += input->getValue();
			++it;
		}
	}

	tx->buildCache();
	mempoolManager->addTransaction(tx);
	// EV_INFO << "New transaction (inputvalue=" << tx->getInputValue().btc() << "; outputvalue=" << tx->getOutputValue().btc() << "; bytes=" << tx->getByteLength() << "; inputs=" << tx->getTxInArraySize() << "; outputs=" << tx->getTxOutArraySize() << "; fee=" << tx->getFee().btc() << ") - Now in mempool: " << mempoolManager->transactionsCount() << endl;
}

}
}
