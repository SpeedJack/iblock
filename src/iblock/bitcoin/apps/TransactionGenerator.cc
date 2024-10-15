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

	txInputValueSignal = registerSignal("txInputValue");
	txOutputValueSignal = registerSignal("txOutputValue");
	txInputCountSignal = registerSignal("txInputCount");
	txOutputCountSignal = registerSignal("txOutputCount");
	txFeeSignal = registerSignal("txFee");
	txSizeSignal = registerSignal("txSize");

	mempoolManager = check_and_cast<MempoolManager*>(getModuleByPath(par("mempoolManagerModule").stringValue()));
	walletManager = check_and_cast<WalletManager*>(getModuleByPath(par("walletManagerModule").stringValue()));
	wallet = check_and_cast<Wallet*>(getModuleByPath(par("walletModule").stringValue()));
	createTransactionMsg = new cMessage("createTransactionMsg");
	minConfirmations = par("minConfirmations").intValue();
	scheduleAt(simTime() + par("interval").doubleValue(), createTransactionMsg);
}

void TransactionGenerator::handleSelfMessage(cMessage *msg)
{
	satoshi_t transactionValue = par("amount");
	if (wallet->balance(minConfirmations) > transactionValue) {
		createTransaction(transactionValue);
		scheduleAt(simTime() + par("interval").doubleValue(), msg);
	} else {
		wallet->notifyOnBalanceIncrease([this, msg](){ Enter_Method_Silent("<callback>()"); this->handleSelfMessage(msg); });
	}
}

void TransactionGenerator::createTransaction(satoshi_t transactionValue)
{
	satoshi_t foundValue = 0;
	std::vector<std::shared_ptr<const TransactionOutput>> allUtxos = wallet->unspentOutputs(minConfirmations);
	std::vector<std::shared_ptr<const TransactionOutput>> utxos;
	std::vector<std::shared_ptr<const TransactionOutput>>::const_iterator it = allUtxos.begin();
	while (foundValue < transactionValue && it != allUtxos.end()) {
		std::shared_ptr<const TransactionOutput> utxo = *it;
		utxos.push_back(utxo);
		foundValue += utxo->getValue();
		++it;
	}
	if (foundValue < transactionValue)
		return;

	std::shared_ptr<Transaction> tx = std::make_shared<Transaction>();
	tx->setVersion(70015);

	tx->setTxInArraySize(utxos.size());
	size_t index = 0;
	for (std::shared_ptr<const TransactionOutput> utxo : utxos) {
		TransactionInput* input = new TransactionInput();
		input->setPrevOutput(std::const_pointer_cast<TransactionOutput>(utxo));
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
		std::shared_ptr<TransactionOutput> output = std::make_shared<TransactionOutput>();
		output->setAddress(walletManager->getRandomBitcoinAddress());
		satoshi_t curValue = transactionValue * splits[i];
		output->setValue(curValue);
		usedValue += curValue;
		tx->setTxOut(index++, output);
	}

	std::shared_ptr<TransactionOutput> change = nullptr;
	while (true) {
		long long remainingValue = foundValue.sat() - usedValue.sat();
		double feeRate = par("feeRate").doubleValue();
		satoshi_t fee = feeRate * tx->getByteLength();
		if (fee < remainingValue) {
			if (!change) {
				change = std::make_shared<TransactionOutput>();
				change->setAddress(wallet->getNewAddress());
				change->setValue(remainingValue - fee.sat());
				tx->appendTxOut(change);
				continue;
			}
			change->setValue(remainingValue - fee.sat());
			break;
		}
		if (fee > remainingValue) {
			if (it == allUtxos.end())
				return;

			std::shared_ptr<TransactionOutput> utxo = std::const_pointer_cast<TransactionOutput>(*it);
			TransactionInput* input = new TransactionInput();
			input->setPrevOutput(utxo);
			tx->appendTxIn(input);
			foundValue += input->getValue();
			++it;
		}
	}

	tx->buildCache();
	emit(txInputValueSignal, tx->getInputValue().sat());
	emit(txOutputValueSignal, tx->getOutputValue().sat());
	emit(txInputCountSignal, tx->getTxInCount());
	emit(txOutputCountSignal, tx->getTxOutCount());
	emit(txFeeSignal, tx->getFee().sat());
	emit(txSizeSignal, static_cast<unsigned int>(tx->getByteLength()));
	mempoolManager->addTransaction(tx);
	// EV_INFO << "New transaction (inputvalue=" << tx->getInputValue().btc() << "; outputvalue=" << tx->getOutputValue().btc() << "; bytes=" << tx->getByteLength() << "; inputs=" << tx->getTxInArraySize() << "; outputs=" << tx->getTxOutArraySize() << "; fee=" << tx->getFee().btc() << ") - Now in mempool: " << mempoolManager->transactionsCount() << endl;
}

}
}
