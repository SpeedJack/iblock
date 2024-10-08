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
		void copy(const Block& other);

	protected:
		std::vector<std::shared_ptr<Transaction>> txn;
		std::unordered_map<Wallet*, std::unordered_set<std::shared_ptr<const TransactionOutput>>*> utxos;

		virtual void addUtxo(std::shared_ptr<const TransactionOutput> utxo);
		virtual void removeUtxo(std::shared_ptr<const TransactionOutput> utxo);
		virtual void updateUtxosOnTransactionAdd(std::shared_ptr<Transaction> tx);
		virtual void updateUtxosOnTransactionRemove(std::shared_ptr<Transaction> tx);
		virtual void eraseUtxos();
		virtual void rebuildUtxos();

	public:
		Block(const char* name = "Block") : Block_Base(name) { setByteLength(80 + 1); }
		Block(BlockHeader* header) : Block() { setHeader(header); }
		Block(const Block& other) : Block_Base(other) { copy(other); }
		Block& operator=(const Block& other) { if (this == &other) return *this; Block_Base::operator=(other); copy(other); return *this; }
		virtual ~Block() override { eraseUtxos(); }

		virtual Block* dup() const override { return new Block(*this); }

		virtual unsigned long getTxnCount() const override { return getTxnArraySize(); }

		virtual const Coinbase* getCoinbaseTx() const { return getCoinbaseTxSharedPtr().get(); }
		virtual std::shared_ptr<const Coinbase> getCoinbaseTxSharedPtr() const { return std::static_pointer_cast<const Coinbase>(getTxnSharedPtr(0)); }

		virtual uint32_t getHeight() const override { return getCoinbaseTxSharedPtr()->getHeight(); }
		virtual satoshi_t getReward() const override { return getCoinbaseTxSharedPtr()->getReward(); }

		virtual std::string str() const override;

		virtual std::shared_ptr<const Block> getPrevBlock() const { return header ? header->getPrevBlock() : nullptr; }

		virtual void setHeader(BlockHeader* header) override;
		virtual BlockHeader* removeHeader() override;
		virtual void replaceHeader(BlockHeader *header);

		virtual void setTxnArraySize(size_t newSize) override;
		virtual size_t getTxnArraySize() const override { return txn.size(); }
		virtual const Transaction* getTxn(size_t k) const override { return txn.at(k).get(); }
		virtual std::shared_ptr<Transaction> getTxnSharedPtr(size_t k) const { return txn.at(k); }
		virtual void setTxn(size_t k, Transaction* txn) override { setTxn(k, std::shared_ptr<Transaction>(txn)); }
		virtual void setTxn(size_t k, std::shared_ptr<Transaction> txn);
		virtual void insertTxn(size_t k, Transaction* txn) override { insertTxn(k, std::shared_ptr<Transaction>(txn)); }
		virtual void insertTxn(size_t k, std::shared_ptr<Transaction> txn);
		virtual void appendTxn(Transaction* txn) override { insertTxn(getTxnArraySize(), std::shared_ptr<Transaction>(txn)); }
		virtual void appendTxn(std::shared_ptr<Transaction> txn) { insertTxn(getTxnArraySize(), txn); }
		virtual void eraseTxn(size_t k) override;

		virtual size_t getUtxoArraySize() const;
		virtual size_t getUtxoCount() const { return getUtxoArraySize(); }
		virtual const std::unordered_set<std::shared_ptr<const TransactionOutput>>* getUtxos(Wallet* wallet) const;
		virtual const std::unordered_set<std::shared_ptr<const TransactionOutput>>* getUtxos(BitcoinAddress* address) const;
};


}
}

namespace omnetpp
{

template<> inline iblock::bitcoin::Block* fromAnyPtr(any_ptr ptr) { return ptr.get<iblock::bitcoin::Block>(); }

}

#endif
