#ifndef __IBLOCK_BITCOIN_PAYLOADS_TX_H_
#define __IBLOCK_BITCOIN_PAYLOADS_TX_H_

#include "TxPl_m.h"
#include "iblock/bitcoin/messages/MessageKind_m.h"

namespace iblock
{
namespace bitcoin
{
namespace payloads
{

class IBLOCK_API TxPl : public TxPl_Base
{
	private:
		void copy(const TxPl& other) { }
	public:
		TxPl() : TxPl_Base() { setKind(MessageKind::BLOCK); setName("tx"); setByteLength(0); }
		TxPl(::iblock::bitcoin::Transaction* transaction) : TxPl() { setTransaction(transaction); }
		TxPl(const TxPl& other) : TxPl_Base(other) { copy(other); }
		TxPl& operator=(const TxPl& other) { if (this == &other) return *this; TxPl_Base::operator=(other); copy(other); return *this; }

		virtual TxPl* dup() const override { return new TxPl(*this); }

		virtual void setTransaction(::iblock::bitcoin::Transaction* transaction) override;

		virtual unsigned char* getRawBytes() const override;

		virtual std::string str() const override
		{
			std::stringstream out;
			std::string baseStr = TxPl_Base::str();
			if (!baseStr.empty())
				out << "; " << baseStr;
			return out.str();
		}
};

}
}
}

namespace omnetpp
{

template<> inline iblock::bitcoin::payloads::TxPl* fromAnyPtr(any_ptr ptr) { return ptr.get<iblock::bitcoin::payloads::TxPl>(); }

}

#endif
