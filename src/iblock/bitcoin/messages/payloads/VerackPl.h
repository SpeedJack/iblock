#ifndef __IBLOCK_BITCOIN_PAYLOADS_VERACKPL_H_
#define __IBLOCK_BITCOIN_PAYLOADS_VERACKPL_H_

#include "VerackPl_m.h"
#include "iblock/bitcoin/messages/MessageKind_m.h"

namespace iblock
{
namespace bitcoin
{
namespace payloads
{

class IBLOCK_API VerackPl : public VerackPl_Base
{
	private:
		void copy(const VerackPl& other) {}
	public:
		VerackPl() : VerackPl_Base() { setKind(MessageKind::VERACK); setName("verack"); setByteLength(0); }
		VerackPl(const VerackPl& other) : VerackPl_Base(other) { copy(other); }
		VerackPl& operator=(const VerackPl& other) { if (this == &other) return *this; VerackPl_Base::operator=(other); copy(other); return *this; }

		virtual VerackPl* dup() const override { return new VerackPl(*this); }

		virtual unsigned char* getRawBytes() const override { return nullptr; }

		virtual std::string str() const override
		{
			std::stringstream out;
			out << "<empty>";
			std::string baseStr = VerackPl_Base::str();
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

template<> inline iblock::bitcoin::payloads::VerackPl* fromAnyPtr(any_ptr ptr) { return ptr.get<iblock::bitcoin::payloads::VerackPl>(); }

}

#endif
