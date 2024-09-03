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
	nodeManager->registerMempoolManager(this);

	mempool.clear();
}

void MempoolManager::handleOtherMessage(cMessage* msg)
{
	EV_DETAIL << "Received a new transaction" << endl;
	appendTransaction(check_and_cast<TxPl*>(msg)->getTransaction());
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
	}
}

void MempoolManager::addTransaction(Transaction* transaction)
{
	Enter_Method("addTransaction()");

	appendTransaction(transaction);

	std::vector<MempoolManager*> nodes = nodeManager->getMempoolManagers();
	for (auto node : nodes) {
		if (node->getId() == this->getId())
			continue;
		cGate* nodeGate = node->gate("dIn");
		sendDirect(new TxPl(transaction), nodeGate);
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
