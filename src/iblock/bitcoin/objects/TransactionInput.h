#ifndef __IBLOCK_BITCOIN_TRANSACTIONINPUT_H_
#define __IBLOCK_BITCOIN_TRANSACTIONINPUT_H_

#include <memory>
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
		Outpoint* prevOutpoint = nullptr;
		std::shared_ptr<TransactionOutput> prevOutput;

		virtual const Transaction* getTransaction() const;
		virtual void updateOutpoint();

	public:
		TransactionInput(const char* name = "TxIn") : TransactionInput_Base(name) { setByteLength(36 + 1 + 108 + 4); }
		TransactionInput(const TransactionInput& other) : TransactionInput_Base(other) { copy(other); }
		TransactionInput& operator=(const TransactionInput& other) { if (this == &other) return *this; TransactionInput_Base::operator=(other); copy(other); return *this; }

		virtual TransactionInput* dup() const override { return new TransactionInput(*this); }

		virtual bool isCoinbase() const { return false; }

		virtual const TransactionOutput* getPrevOutput() const override { return prevOutput.get(); }
		virtual void setPrevOutput(TransactionOutput* prevOutput) override { this->prevOutput.reset(prevOutput); }
		virtual void setPrevOutput(std::shared_ptr<TransactionOutput> prevOutput) { this->prevOutput = prevOutput; }
		virtual std::shared_ptr<TransactionOutput> getPrevOutputSharedPtr() const { return prevOutput; }

		virtual const Outpoint& getPrevOutpoint() const override { return *prevOutpoint; }
		virtual unsigned int getScriptBytes() const override { return strlen(getSignatureScript()); }
		virtual const char* getSignatureScript() const override { return ""; }

		virtual satoshi_t getValue() const override { if (prevOutput) return prevOutput->getValue(); return 0; }

		virtual std::string str() const override;

		virtual ~TransactionInput() { if (prevOutpoint) delete prevOutpoint; }
};

}
}

namespace omnetpp
{

template<> inline iblock::bitcoin::TransactionInput* fromAnyPtr(any_ptr ptr) { return ptr.get<iblock::bitcoin::TransactionInput>(); }

}

#endif
