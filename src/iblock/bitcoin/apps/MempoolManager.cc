#include "MempoolManager.h"

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

	nodeManager = check_and_cast<NodeManager *>(getModuleByPath(par("nodeManagerModule").stringValue()));
	nodeManager->registerMempoolManager(this);

	mempool.clear();
}

void MempoolManager::handleOtherMessage(cMessage *msg)
{
	EV << "Received a new transaction" << endl;
	appendTransaction(check_and_cast<TxPl *>(msg)->getTransaction());
	delete msg;
}

void MempoolManager::removeTransactions(const Block *block)
{
	Enter_Method("removeTransactions()");
	size_t count = block->getTxnArraySize();
	for (size_t i = 0; i< count; i++)
		removeTransaction(block->getTxn(i));

}

void MempoolManager::addTransaction(Transaction *transaction)
{
	Enter_Method("addTransaction()");

	if (mempool.find(transaction) != mempool.end())
		return;
	mempool.insert(transaction);

	std::vector<MempoolManager *> nodes = nodeManager->getMempoolManagers();
	for (auto node : nodes) {
		if (node->getId() == this->getId())
			continue;
		cGate *nodeGate = node->gate("dIn");
		sendDirect(new TxPl(transaction), nodeGate);
	}
}

const std::vector<Transaction *> MempoolManager::getTransactions(int64_t maxBytes) const
{
	Enter_Method("getTransactions()");

	std::vector<Transaction *> txns;
	for (auto tx : mempool) {
		maxBytes -= tx->getSerializedSize();
		if (maxBytes < 0)
			break;
		txns.push_back(const_cast<Transaction *>(tx));
	}
	return txns;
}

}
}
