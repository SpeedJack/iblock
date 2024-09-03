#ifndef __IBLOCK_BITCOIN_PAYLOADS_INV_H_
#define __IBLOCK_BITCOIN_PAYLOADS_INV_H_

#include "InvPl_m.h"
#include "iblock/bitcoin/messages/MessageKind_m.h"

namespace iblock
{
namespace bitcoin
{
namespace payloads
{

class IBLOCK_API InvPl : public InvPl_Base
{
	private:
		void copy(const InvPl& other) { }
	public:
		InvPl() : InvPl_Base() { setKind(MessageKind::BLOCK); setName("inv"); setByteLength(0); }
		InvPl(const InvPl& other) : InvPl_Base(other) { copy(other); }
		InvPl& operator=(const InvPl& other) { if (this == &other) return *this; InvPl_Base::operator=(other); copy(other); return *this; }

		virtual InvPl* dup() const override { return new InvPl(*this); }

		virtual unsigned char* getRawBytes() const override;

		virtual std::string str() const override
		{
			std::stringstream out;
			std::string baseStr = InvPl_Base::str();
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

template<> inline iblock::bitcoin::payloads::InvPl* fromAnyPtr(any_ptr ptr) { return ptr.get<iblock::bitcoin::payloads::InvPl>(); }

}

#endif
