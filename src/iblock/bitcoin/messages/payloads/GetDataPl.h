#ifndef __IBLOCK_BITCOIN_PAYLOADS_GETDATA_H_
#define __IBLOCK_BITCOIN_PAYLOADS_GETDATA_H_

#include "GetDataPl_m.h"
#include "iblock/bitcoin/messages/MessageKind_m.h"

namespace iblock
{
namespace bitcoin
{
namespace payloads
{

class IBLOCK_API GetDataPl : public GetDataPl_Base
{
	private:
		void copy(const GetDataPl& other) { }
	public:
		GetDataPl() : GetDataPl_Base() { setKind(MessageKind::BLOCK); setName("getdata"); setByteLength(0); }
		GetDataPl(const GetDataPl& other) : GetDataPl_Base(other) { copy(other); }
		GetDataPl& operator=(const GetDataPl& other) { if (this == &other) return *this; GetDataPl_Base::operator=(other); copy(other); return *this; }

		virtual GetDataPl *dup() const override { return new GetDataPl(*this); }

		virtual unsigned char *getRawBytes() const override;

		virtual std::string str() const override
		{
			std::stringstream out;
			std::string baseStr = GetDataPl_Base::str();
			if (!baseStr.empty())
				out << "; " << baseStr;
			return out.str();
		}
};

}
}
}

#endif
