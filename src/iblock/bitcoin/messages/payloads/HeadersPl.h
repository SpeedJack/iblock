#ifndef __IBLOCK_BITCOIN_PAYLOADS_HEADERS_H_
#define __IBLOCK_BITCOIN_PAYLOADS_HEADERS_H_

#include "HeadersPl_m.h"
#include "iblock/bitcoin/messages/MessageKind_m.h"

namespace iblock
{
namespace bitcoin
{
namespace payloads
{

class IBLOCK_API HeadersPl : public HeadersPl_Base
{
	private:
		void copy(const HeadersPl& other) { }
	public:
		HeadersPl() : HeadersPl_Base() { setKind(MessageKind::BLOCK); setName("headers"); setByteLength(0); }
		HeadersPl(const HeadersPl& other) : HeadersPl_Base(other) { copy(other); }
		HeadersPl& operator=(const HeadersPl& other) { if (this == &other) return *this; HeadersPl_Base::operator=(other); copy(other); return *this; }

		virtual HeadersPl *dup() const override { return new HeadersPl(*this); }

		virtual unsigned char *getRawBytes() const override;

		virtual std::string str() const override
		{
			std::stringstream out;
			std::string baseStr = HeadersPl_Base::str();
			if (!baseStr.empty())
				out << "; " << baseStr;
			return out.str();
		}
};

}
}
}

#endif
