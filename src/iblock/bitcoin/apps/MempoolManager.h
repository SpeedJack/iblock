#ifndef __IBLOCK_BITCOIN_MEMPOOLMANAGER_H_
#define __IBLOCK_BITCOIN_MEMPOOLMANAGER_H_

#include "iblock/bitcoin/apps/base/AppBase.h"
#include "iblock/bitcoin/global/NodeManager.h"
#include "iblock/bitcoin/objects/Block.h"
#include <cppcoro/generator.hpp>

namespace iblock
{
namespace bitcoin
{

struct TransactionCmp
{
	bool operator()(const Transaction* lhs, const Transaction* rhs) const
	{
		return lhs->getFeeRate() > rhs->getFeeRate();
	}
};

class IBLOCK_API MempoolManager : public AppBase
{
	protected:
		NodeManager* nodeManager;
		std::set<const Transaction*, TransactionCmp> mempool;
		std::vector<Wallet *> wallets = std::vector<Wallet *>();

		virtual void initialize() override;
		virtual void handleOtherMessage(::omnetpp::cMessage* msg) override;
		virtual void removeTransaction(const Transaction* transaction) { if(transaction) mempool.erase(transaction); }
		virtual void appendTransaction(const Transaction* transaction);
	public:
		MempoolManager() : AppBase() { nodeManager = nullptr; };
		virtual void addTransaction(Transaction* transaction);
		virtual void addBlockTransactions(const Block* block);
		virtual void removeBlockTransactions(const Block* block);
		virtual cppcoro::generator<const Transaction*> transactions() const;
		void registerWallet(Wallet* wallet) { Enter_Method_Silent("registerWallet()"); wallets.push_back(wallet); }
};

}
}

#endif
