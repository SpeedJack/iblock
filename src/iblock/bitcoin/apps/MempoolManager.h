#ifndef __IBLOCK_BITCOIN_MEMPOOLMANAGER_H_
#define __IBLOCK_BITCOIN_MEMPOOLMANAGER_H_

#include "iblock/bitcoin/apps/base/AppBase.h"
#include "iblock/bitcoin/global/NodeManager.h"
#include "iblock/bitcoin/objects/Block.h"

namespace iblock
{
namespace bitcoin
{

struct TransactionCmp
{
	bool operator()(const Transaction *lhs, const Transaction *rhs) const
	{
		return lhs->getTotalOutputValue() < rhs->getTotalOutputValue();
	}
};

class IBLOCK_API MempoolManager : public AppBase
{
	protected:
		NodeManager *nodeManager;
		std::set<const Transaction *, TransactionCmp> mempool;

		virtual void initialize() override;
		virtual void handleOtherMessage(::omnetpp::cMessage *msg) override;
		virtual void removeTransaction(const Transaction *transaction) { mempool.erase(transaction); }
		virtual void appendTransaction(const Transaction *transaction) { mempool.insert(transaction); }
	public:
		MempoolManager() : AppBase() { nodeManager = nullptr; };
		virtual void addTransaction(Transaction *transaction);
		virtual void removeTransactions(const Block *block);
		virtual const std::vector<Transaction *> getTransactions(int64_t maxBytes = 1000*1000) const;
};

}
}

#endif
