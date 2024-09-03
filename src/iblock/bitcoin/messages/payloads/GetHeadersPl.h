#ifndef __IBLOCK_BITCOIN_PAYLOADS_GETHEADERS_H_
#define __IBLOCK_BITCOIN_PAYLOADS_GETHEADERS_H_

#include "GetHeadersPl_m.h"
#include "iblock/bitcoin/messages/MessageKind_m.h"

namespace iblock
{
namespace bitcoin
{
namespace payloads
{

class IBLOCK_API GetHeadersPl : public GetHeadersPl_Base
{
	private:
		void copy(const GetHeadersPl& other) { }
	public:
		GetHeadersPl() : GetHeadersPl_Base() { setKind(MessageKind::BLOCK); setName("getheaders"); setByteLength(0); }
		GetHeadersPl(const GetHeadersPl& other) : GetHeadersPl_Base(other) { copy(other); }
		GetHeadersPl& operator=(const GetHeadersPl& other) { if (this == &other) return *this; GetHeadersPl_Base::operator=(other); copy(other); return *this; }

		virtual GetHeadersPl* dup() const override { return new GetHeadersPl(*this); }

		virtual unsigned char* getRawBytes() const override;

		virtual std::string str() const override
		{
			std::stringstream out;
			std::string baseStr = GetHeadersPl_Base::str();
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

template<> inline iblock::bitcoin::payloads::GetHeadersPl* fromAnyPtr(any_ptr ptr) { return ptr.get<iblock::bitcoin::payloads::GetHeadersPl>(); }

}

#endif
