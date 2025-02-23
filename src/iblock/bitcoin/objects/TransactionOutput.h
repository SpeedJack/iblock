#ifndef __IBLOCK_BITCOIN_TRANSACTIONOUTPUT_H_
#define __IBLOCK_BITCOIN_TRANSACTIONOUTPUT_H_

#include "TransactionOutput_m.h"

namespace iblock
{
namespace bitcoin
{

class Transaction;

class IBLOCK_API TransactionOutput : public TransactionOutput_Base
{
	private:
		void copy(const TransactionOutput& other) { this->id = nextId++; }

	protected:
		static unsigned long long nextId;
		unsigned long long id;
		bool coinbase = false;

		// virtual const Transaction* getTransaction() const;

	public:
		TransactionOutput(const char* name = "TxOut") : TransactionOutput_Base(name) { setByteLength(8 + compactSize(getPkScriptBytes()) + getPkScriptBytes()); id = nextId++; }
		TransactionOutput(BitcoinAddress* address, satoshi_t value) : TransactionOutput() { setAddress(address); setValue(value); }
		TransactionOutput(const TransactionOutput& other) : TransactionOutput_Base(other) { copy(other); }
		TransactionOutput & operator=(const TransactionOutput& other) { if (this == &other) return *this; TransactionOutput_Base::operator=(other); copy(other); return *this; }

		virtual TransactionOutput* dup() const override { return new TransactionOutput(*this); }

		virtual unsigned long long getId() const { return id; }

		virtual bool isCoinbase() const { return coinbase; }
		virtual void setCoinbase(bool coinbase = true) { this->coinbase = coinbase; }

		virtual unsigned int getPkScriptBytes() const override { return strlen(getPkScript()); }

		virtual const char* getPkScript() const override { return "abcdefghijklmnopqrstuvwxy"; }

		virtual std::string str() const override;
};

}
}

namespace omnetpp
{

template<> inline iblock::bitcoin::TransactionOutput* fromAnyPtr(any_ptr ptr) { return ptr.get<iblock::bitcoin::TransactionOutput>(); }

}

#endif
