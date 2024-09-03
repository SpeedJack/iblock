#ifndef __IBLOCK_BITCOIN_WALLET_H_
#define __IBLOCK_BITCOIN_WALLET_H_

#include "iblock/bitcoin/apps/base/AppBase.h"
#include "iblock/bitcoin/common/BitcoinAddress.h"
#include "iblock/bitcoin/global/NodeManager.h"

namespace iblock
{
namespace bitcoin
{

struct UTXOCmp
{
	bool operator()(const TransactionOutput* a, const TransactionOutput* b) const
	{
		return a->getValue() < b->getValue();
	}
};

class IBLOCK_API Wallet : public AppBase
{
	protected:
		NodeManager* nodeManager;
		uint32_t nextAddressIndex;
		BlockchainManager* blockchainManager;
		MempoolManager* mempoolManager;
		std::map<const TransactionOutput*, uint32_t, UTXOCmp> utxos;

		virtual void initialize() override;

	public:
		Wallet() : AppBase() { mempoolManager = nullptr; blockchainManager = nullptr; nodeManager = nullptr; }

		virtual BitcoinAddress* getNewAddress();

		virtual void addUtxo(const TransactionOutput* utxo);
		virtual void confirmUtxo(const TransactionOutput* utxo);
		virtual void unconfirmUtxo(const TransactionOutput* utxo);

		virtual std::vector<const TransactionOutput *> unspentOutputs(int64_t minValue, uint32_t minConfirmations = 1) const;

		virtual int64_t balance(uint32_t minConfirmations = 1) const;
};

}
}

#endif
