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

	public:
		Transaction(const char* name = "Transaction") : Transaction_Base(name) { setByteLength(4 + 1 + 0 + 1 + 0 + 4); }
		Transaction(const Transaction& other) : Transaction_Base(other) { copy(other); }
		Transaction& operator=(const Transaction& other) { if (this == &other) return *this; Transaction_Base::operator=(other); copy(other); return *this; }

		virtual Transaction* dup() const override { return new Transaction(*this); }

		virtual ObjectType getType() const override { return ObjectType::MSG_TX; }

		virtual unsigned long getTxInCount() const override { return getTxInArraySize(); }
		virtual unsigned long getTxOutCount() const override { return getTxOutArraySize(); }

		virtual void setTxInArraySize(size_t size) override;
		virtual void setTxIn(size_t k, TransactionInput* txIn) override;
		virtual TransactionInput* removeTxIn(size_t k) override;
		virtual void insertTxIn(size_t k, TransactionInput* txIn) override;
		virtual void eraseTxIn(size_t k) override;

		virtual void setTxOutArraySize(size_t size) override;
		virtual void setTxOut(size_t k, TransactionOutput* txOut) override;
		virtual TransactionOutput* removeTxOut(size_t k) override;
		virtual void insertTxOut(size_t k, TransactionOutput* txOut) override;
		virtual void eraseTxOut(size_t k) override;

		virtual int64_t getOutputValue() const override;
		virtual int64_t getInputValue() const override;
		virtual int64_t getFee() const override { return getInputValue() - getOutputValue(); }
		virtual double getFeeRate() const override { return getFee() / (double) getWeight(); }
		virtual unsigned long getWeight() const { return getByteLength(); }

		virtual std::string str() const override;
};

}
}

namespace omnetpp
{

template<> inline iblock::bitcoin::Transaction* fromAnyPtr(any_ptr ptr) { return ptr.get<iblock::bitcoin::Transaction>(); }

}

#endif
