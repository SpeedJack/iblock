#ifndef __IBLOCK_BITCOIN_TRANSACTION_H_
#define __IBLOCK_BITCOIN_TRANSACTION_H_

#include "Transaction_m.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API Transaction : public Transaction_Base
{
	private:
		void copy(const Transaction& other);

	protected:
		unsigned long long outputValueCache = 0;
		unsigned long long inputValueCache = 0;
		unsigned long long fee = 0;
		double feeRate = -1;
		std::vector<std::shared_ptr<TransactionOutput>> txOut;

	public:
		Transaction(const char* name = "Transaction") : Transaction_Base(name) { setByteLength(4 + 1 + 0 + 1 + 0 + 4); }
		Transaction(const Transaction& other) : Transaction_Base(other) { copy(other); }
		Transaction& operator=(const Transaction& other) { if (this == &other) return *this; Transaction_Base::operator=(other); copy(other); return *this; }

		virtual Transaction* dup() const override { return new Transaction(*this); }

		virtual ObjectType getType() const override { return ObjectType::MSG_TX; }

		virtual bool isCoinbase() const { return getTxInCount() == 1 && getTxIn(0)->isCoinbase(); }

		virtual unsigned long getTxInCount() const override { return getTxInArraySize(); }
		virtual unsigned long getTxOutCount() const override { return getTxOutArraySize(); }

		virtual void setTxInArraySize(size_t size) override;
		virtual void setTxIn(size_t k, TransactionInput* txIn) override;
		virtual TransactionInput* removeTxIn(size_t k) override;
		virtual void insertTxIn(size_t k, TransactionInput* txIn) override;
		virtual void eraseTxIn(size_t k) override;

		virtual void setTxOutArraySize(size_t size) override;
		virtual size_t getTxOutArraySize() const override { return txOut.size(); }
		virtual const TransactionOutput* getTxOut(size_t k) const override { return txOut.at(k).get(); }
		virtual std::shared_ptr<TransactionOutput> getTxOutSharedPtr(size_t k) const { return txOut.at(k); }
		virtual void setTxOut(size_t k, std::shared_ptr<TransactionOutput> txOut);
		virtual void setTxOut(size_t k, TransactionOutput* txOut) override { setTxOut(k, std::shared_ptr<TransactionOutput>(txOut)); }
		virtual void insertTxOut(size_t k, std::shared_ptr<TransactionOutput> txOut);
		virtual void insertTxOut(size_t k, TransactionOutput* txOut) override { insertTxOut(k, std::shared_ptr<TransactionOutput>(txOut)); }
		virtual void appendTxOut(TransactionOutput* txOut) override { insertTxOut(getTxOutArraySize(), txOut); }
		virtual void appendTxOut(std::shared_ptr<TransactionOutput> txOut) { insertTxOut(getTxOutArraySize(), txOut); }
		virtual void eraseTxOut(size_t k) override;

		virtual satoshi_t getOutputValue() const override;
		virtual satoshi_t getInputValue() const override;
		virtual satoshi_t getFee() const override { if (fee > 0) return fee; return getInputValue() - getOutputValue(); }
		virtual double getFeeRate() const override { if (feeRate >= 0) return feeRate; return getFee() / (double)getWeight(); }
		virtual unsigned long getWeight() const { return getByteLength(); }

		virtual void invalidateCache() { inputValueCache = 0; outputValueCache = 0; fee = 0; feeRate = -1; }
		virtual void updateCache() { inputValueCache = getInputValue().sat(); outputValueCache = getOutputValue().sat(); fee = getFee().sat(); feeRate = getFeeRate(); }
		virtual void buildCache() { if (feeRate < 0) updateCache(); }

		virtual std::string str() const override;
};

}
}

namespace omnetpp
{

template<> inline iblock::bitcoin::Transaction* fromAnyPtr(any_ptr ptr) { return ptr.get<iblock::bitcoin::Transaction>(); }

}

#endif
