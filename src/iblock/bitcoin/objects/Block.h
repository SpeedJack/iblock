#ifndef __IBLOCK_BITCOIN_BLOCK_H_
#define __IBLOCK_BITCOIN_BLOCK_H_

#include "Block_m.h"
#include "Coinbase.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API Block : public Block_Base
{
	private:
		void copy(const Block& other) { };

	protected:
		virtual void addUtxo(const TransactionOutput* utxo);
		virtual void removeUtxo(const TransactionOutput* utxo);
		virtual void updateUtxosOnTransactionAdd(const Transaction* tx);
		virtual void updateUtxosOnTransactionRemove(const Transaction *tx);
		virtual void eraseUtxos();
		virtual void rebuildUtxos();

	public:
		Block(const char* name = "Block") : Block_Base(name) { setByteLength(80 + 1); }
		Block(BlockHeader* header) : Block() { setHeader(header); }
		Block(const Block& other) : Block_Base(other) { copy(other); }
		Block& operator=(const Block& other) { if (this == &other) return *this; Block_Base::operator=(other); copy(other); return *this; }

		virtual Block* dup() const override { return new Block(*this); }

		virtual unsigned long getTxnCount() const override { return getTxnArraySize(); }

		virtual const Coinbase* getCoinbaseTx() const { return static_cast<const Coinbase*>(getTxn(0)); }

		virtual uint32_t getHeight() const override { return ::omnetpp::check_and_cast<const Coinbase*>(getCoinbaseTx())->getHeight(); }
		virtual satoshi_t getReward() const override { return getCoinbaseTx()->getReward(); }

		virtual std::string str() const override;

		virtual const Block* getPrevBlock() const { const BlockHeader* h = getHeader(); if (h) return h->getPrevBlock(); else return nullptr; }

		virtual void setHeader(BlockHeader* header) override;
		virtual BlockHeader* removeHeader() override;
		virtual void replaceHeader(BlockHeader *header);

		virtual void setTxnArraySize(size_t newSize) override;
		virtual void setTxn(size_t k, Transaction* txn) override;
		virtual void insertTxn(size_t k, Transaction* txn) override;
		virtual void eraseTxn(size_t k) override;

		virtual size_t getUtxoArraySize() const override;
		virtual const TransactionOutput* getUtxo(size_t k) const override;
		virtual const std::unordered_set<const TransactionOutput*>* getUtxos(Wallet* wallet) const override;
		virtual const std::unordered_set<const TransactionOutput*>* getUtxos(BitcoinAddress* address) const override;
		virtual bool hasUtxo(const TransactionOutput* utxo) const override;
};


}
}

namespace omnetpp
{

template<> inline iblock::bitcoin::Block* fromAnyPtr(any_ptr ptr) { return ptr.get<iblock::bitcoin::Block>(); }

}

#endif
