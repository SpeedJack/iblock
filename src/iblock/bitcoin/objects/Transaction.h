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
		Transaction(const char *name = "Transaction") : Transaction_Base(name) { }
		Transaction(const Transaction& other) : Transaction_Base(other) { copy(other); }
		Transaction& operator=(const Transaction& other) { if (this == &other) return *this; Transaction_Base::operator=(other); copy(other); return *this; }

		virtual Transaction *dup() const override { return new Transaction(*this); }

		virtual ObjectType getType() const override { return ObjectType::MSG_TX; }

		virtual unsigned long getTxInCount() const override { return getTxInArraySize(); }
		virtual unsigned long getTxOutCount() const override { return getTxOutArraySize(); }

		virtual std::string str() const override;
};

}
}

namespace omnetpp
{

template<> inline iblock::bitcoin::Transaction *fromAnyPtr(any_ptr ptr) { return check_and_cast<iblock::bitcoin::Transaction *>(ptr.get<cObject>()); }

}

#endif
