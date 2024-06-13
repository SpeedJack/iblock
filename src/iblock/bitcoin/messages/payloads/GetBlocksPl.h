#ifndef __IBLOCK_BITCOIN_PAYLOADS_GETBLOCKS_H_
#define __IBLOCK_BITCOIN_PAYLOADS_GETBLOCKS_H_

#include "GetBlocksPl_m.h"
#include "iblock/bitcoin/messages/MessageKind_m.h"

namespace iblock
{
namespace bitcoin
{
namespace payloads
{

class IBLOCK_API GetBlocksPl : public GetBlocksPl_Base
{
	private:
		void copy(const GetBlocksPl& other) { }
	public:
		GetBlocksPl() : GetBlocksPl_Base() { setKind(MessageKind::BLOCK); setName("getblocks"); setByteLength(0); }
		GetBlocksPl(const GetBlocksPl& other) : GetBlocksPl_Base(other) { copy(other); }
		GetBlocksPl& operator=(const GetBlocksPl& other) { if (this == &other) return *this; GetBlocksPl_Base::operator=(other); copy(other); return *this; }

		virtual GetBlocksPl *dup() const override { return new GetBlocksPl(*this); }

		virtual unsigned char *getRawBytes() const override;

		virtual std::string str() const override
		{
			std::stringstream out;
			std::string baseStr = GetBlocksPl_Base::str();
			if (!baseStr.empty())
				out << "; " << baseStr;
			return out.str();
		}
};

}
}
}

#endif
