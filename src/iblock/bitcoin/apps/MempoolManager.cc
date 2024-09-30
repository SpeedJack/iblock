#include "MempoolManager.h"
#include "Wallet.h"

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

	nodeManager = check_and_cast<NodeManager*>(getModuleByPath(par("nodeManagerModule").stringValue()));
	gmm = check_and_cast<GMM*>(getModuleByPath(par("gmmModule").stringValue()));

	mempool.clear();
}

void MempoolManager::handleOtherMessage(cMessage* msg)
{
	// size_t before = transactionsCount();
	appendTransaction(check_and_cast<TxPl*>(msg)->getTransaction());
	// EV_INFO << "Received a new transaction " << before << " --> "<< transactionsCount() << endl;
	delete msg;
}

void MempoolManager::removeBlockTransactions(const Block* block)
{
	Enter_Method("removeBlockTransactions()");
	size_t count = block->getTxnArraySize();
	for (size_t i = 0; i < count; i++)
		removeTransaction(block->getTxn(i));

}

void MempoolManager::addBlockTransactions(const Block* block)
{
	Enter_Method("addBlockTransactions()");
	size_t count = block->getTxnArraySize();
	for (size_t i = 0; i < count; i++)
		appendTransaction(block->getTxn(i));
}

void MempoolManager::appendTransaction(const Transaction* transaction)
{
	mempool.insert(transaction);
	for (Wallet* wallet : wallets) {
		size_t txoCount = transaction->getTxOutArraySize();
		for (size_t i = 0; i < txoCount; i++) {
			const TransactionOutput* txo = transaction->getTxOut(i);
			if (wallet == txo->getAddress()->getWallet())
				wallet->addUtxo(txo);
		}
		if (transaction->isCoinbase())
			continue;
		size_t txiCount = transaction->getTxInArraySize();
		for (size_t i = 0; i < txiCount; i++) {
			const TransactionInput* txi = transaction->getTxIn(i);
			const TransactionOutput* txo = txi->getPrevOutput();
			if (wallet == txo->getAddress()->getWallet())
				wallet->removeUtxo(txo);
		}
	}
}

void MempoolManager::addTransaction(Transaction* transaction)
{
	Enter_Method("addTransaction()");

	gmm->addTransaction(transaction);
	appendTransaction(transaction);
	if (transaction->isCoinbase())
		return;

	for (auto node : nodeManager->nodes()) {
		if (node->getId() == this->getParentModule()->getId())
			continue;
		cGate* nodeGate = node->gate("mempoolManagerIn");
		// sendDirect(new TxPl(transaction), nodeGate);
		sendDirect(new TxPl(transaction), exponential(5.1), transaction->getBitLength() / ((double)1000*1000), nodeGate);
	}
}

cppcoro::generator<const Transaction*> MempoolManager::transactions() const
{
	Enter_Method("transactions()");

	for (auto tx : mempool)
		co_yield tx;
}

}
}
