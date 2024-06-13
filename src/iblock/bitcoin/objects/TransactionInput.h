#ifndef __IBLOCK_BITCOIN_TRANSACTIONINPUT_H_
#define __IBLOCK_BITCOIN_TRANSACTIONINPUT_H_

#include "TransactionInput_m.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API TransactionInput : public TransactionInput_Base
{
	private:
		void copy(const TransactionInput& other);

	protected:
		Outpoint *prevOutpoint;

		virtual void updateOutpoint();

	public:
		TransactionInput(const char *name = "TxIn") : TransactionInput_Base(name) { }
		TransactionInput(const TransactionInput& other) : TransactionInput_Base(other) { copy(other); }
		TransactionInput& operator=(const TransactionInput& other) { if (this == &other) return *this; TransactionInput_Base::operator=(other); copy(other); return *this; }

		virtual TransactionInput *dup() const override { return new TransactionInput(*this); }

		virtual const Outpoint& getPrevOutpoint() const override { return *prevOutpoint; }
		virtual unsigned int getScriptBytes() const override { return strlen(getSignatureScript()); }
		virtual const char *getSignatureScript() const override { return ""; }

		virtual std::string str() const override;
};

}
}

namespace omnetpp
{

template<> inline iblock::bitcoin::TransactionInput *fromAnyPtr(any_ptr ptr) { return check_and_cast<iblock::bitcoin::TransactionInput *>(ptr.get<cObject>()); }

}

#endif
