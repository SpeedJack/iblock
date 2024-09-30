#ifndef __IBLOCK_BITCOIN_WALLET_H_
#define __IBLOCK_BITCOIN_WALLET_H_

#include "iblock/bitcoin/apps/base/AppBase.h"
#include "iblock/bitcoin/common/BitcoinAddress.h"

namespace iblock
{
namespace bitcoin
{

class BlockchainManager;
class MempoolManager;

struct UTXOCmp
{
	bool operator()(const TransactionOutput* a, const TransactionOutput* b) const
	{
		satoshi_t aValue = a->getValue();
		satoshi_t bValue = b->getValue();
		if (aValue == bValue)
			return a > b;
		return aValue > bValue;
	}
};

class IBLOCK_API Wallet : public AppBase
{
	protected:
		uint32_t nextAddressIndex;
		BlockchainManager* blockchainManager;
		MempoolManager* mempoolManager;
		std::map<const TransactionOutput*, uint32_t, UTXOCmp> utxos;
		unsigned long long confirmedBalance;
		unsigned long long totalBalance;

		virtual void initialize() override;

	public:
		Wallet() : AppBase() { mempoolManager = nullptr; blockchainManager = nullptr; }

		virtual BitcoinAddress* getNewAddress();

		virtual void addUtxo(const TransactionOutput* utxo);
		virtual void removeUtxo(const TransactionOutput* utxo);
		virtual void confirmUtxo(const TransactionOutput* utxo);
		virtual void unconfirmUtxo(const TransactionOutput* utxo);

		virtual std::vector<const TransactionOutput*> unspentOutputs(uint32_t minConfirmations = 1) const;

		virtual satoshi_t balance(uint32_t minConfirmations = 1) const;
		virtual bool mayHaveBalance(satoshi_t amount, uint32_t minConfirmations = 1) const { Enter_Method_Silent("mayHaveBalance()"); if (minConfirmations == 0) return totalBalance >= amount; return confirmedBalance >= amount; }
};

}
}

#endif
