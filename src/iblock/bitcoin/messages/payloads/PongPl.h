#ifndef __IBLOCK_BITCOIN_PAYLOADS_PONG_H_
#define __IBLOCK_BITCOIN_PAYLOADS_PONG_H_

#include "PongPl_m.h"
#include "iblock/bitcoin/messages/MessageKind_m.h"

namespace iblock
{
namespace bitcoin
{
namespace payloads
{

class PongPl : public PongPl_Base
{
	private:
		void copy(const PongPl& other) {}
	public:
		PongPl() : PongPl_Base() { setKind(MessageKind::PONG); setName("pong"); }
		PongPl(uint64_t nonce) : PongPl() { setNonce(nonce); }
		PongPl(const PongPl& other) : PongPl_Base(other) { copy(other); }
		PongPl& operator=(const PongPl& other) { if (this == &other) return *this; PongPl_Base::operator=(other); copy(other); return *this; }

		virtual PongPl *dup() const override { return new PongPl(*this); }

		virtual unsigned char *getRawBytes() const override { return nullptr; }

		virtual std::string str() const override
		{
			std::stringstream out;
			out << "nonce=" << getNonce();
			std::string baseStr = PongPl_Base::str();
			if (!baseStr.empty())
				out << "; " << baseStr;
			return out.str();
		}
};

}
}
}

#endif
