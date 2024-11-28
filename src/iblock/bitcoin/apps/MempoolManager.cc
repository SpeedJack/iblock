#include "MempoolManager.h"
#include "Wallet.h"
#include "iblock/bitcoin/messages/DirectTxMsg.h"

using namespace omnetpp;
using namespace iblock::bitcoin::payloads;

Define_Module(iblock::bitcoin::MempoolManager);

namespace iblock
{
namespace bitcoin
{

void MempoolManager::initialize()
{
	AppBase::initialize();

	addTransactionSignal = registerSignal("transactionAdded");
	removeTransactionSignal = registerSignal("transactionRemoved");
	mempoolSizeSignal = registerSignal("mempoolSize");

	nodeManager = check_and_cast<NodeManager*>(getModuleByPath(par("nodeManagerModule").stringValue()));

	mempool.clear();
}

void MempoolManager::handleOtherMessage(cMessage* msg)
{
	// size_t before = transactionsCount();
	appendTransaction(check_and_cast<DirectTxMsg*>(msg)->getTxSharedPtr());
	// EV_INFO << "Received a new transaction " << before << " --> "<< transactionsCount() << endl;
	delete msg;
}

void MempoolManager::removeBlockTransactions(std::shared_ptr<const Block> block)
{
	Enter_Method("removeBlockTransactions()");
	size_t count = block->getTxnArraySize();
	for (size_t i = 0; i < count; i++)
		removeTransaction(block->getTxnSharedPtr(i));

}

void MempoolManager::addBlockTransactions(std::shared_ptr<const Block> block)
{
	Enter_Method("addBlockTransactions()");
	size_t count = block->getTxnArraySize();
	for (size_t i = 0; i < count; i++)
		appendTransaction(block->getTxnSharedPtr(i));
}

void MempoolManager::appendTransaction(std::shared_ptr<const Transaction> transaction)
{
	mempool.insert(transaction);
	emit(addTransactionSignal, 1U);
	emit(mempoolSizeSignal, transaction->getByteLength());
	for (Wallet* wallet : wallets) {
		size_t txoCount = transaction->getTxOutArraySize();
		for (size_t i = 0; i < txoCount; i++) {
			std::shared_ptr<const TransactionOutput> txo = transaction->getTxOutSharedPtr(i);
			if (wallet == txo->getAddress()->getWallet())
				wallet->addUtxo(txo);
		}
		if (transaction->isCoinbase())
			continue;
		size_t txiCount = transaction->getTxInArraySize();
		for (size_t i = 0; i < txiCount; i++) {
			const TransactionInput* txi = transaction->getTxIn(i);
			std::shared_ptr<const TransactionOutput> txo = txi->getPrevOutputSharedPtr();
			if (wallet == txo->getAddress()->getWallet())
				wallet->removeUtxo(txo);
		}
	}
}

void MempoolManager::addTransaction(std::shared_ptr<Transaction> transaction)
{
	Enter_Method("addTransaction()");

	appendTransaction(std::const_pointer_cast<const Transaction>(transaction));
	if (transaction->isCoinbase())
		return;

	for (auto node : nodeManager->nodes()) {
		if (node->getId() == this->getParentModule()->getId())
			continue;
		cGate* nodeGate = node->gate("mempoolManagerIn");
		// sendDirect(new TxPl(transaction), nodeGate);
		double delay = par("propagationDelay").doubleValue();
		double txDuration = transaction->getBitLength() / par("bandwidth").doubleValue();
		sendDirect(new DirectTxMsg(transaction), delay, txDuration, nodeGate);
	}
}

void MempoolManager::removeTransaction(std::shared_ptr<const Transaction> transaction)
{
	if (!transaction)
		return;
	mempool.erase(transaction);
	emit(removeTransactionSignal, 1U);
	emit(mempoolSizeSignal, -transaction->getByteLength());
}

cppcoro::generator<std::shared_ptr<const Transaction>> MempoolManager::transactions() const
{
	Enter_Method("transactions()");

	for (auto tx : mempool)
		co_yield tx;
}

}
}
