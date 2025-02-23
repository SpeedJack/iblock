#ifndef __IBLOCK_BITCOIN_PAYLOADS_PING_H_
#define __IBLOCK_BITCOIN_PAYLOADS_PING_H_

#include "PingPl_m.h"
#include "iblock/bitcoin/messages/MessageKind_m.h"

namespace iblock
{
namespace bitcoin
{
namespace payloads
{

class IBLOCK_API PingPl : public PingPl_Base
{
	private:
		void copy(const PingPl& other) { nonce = other.nonce; }
	public:
		PingPl() : PingPl_Base() { setKind(MessageKind::PING); setName("ping"); setByteLength(8); }
		PingPl(uint64_t nonce) : PingPl() { setNonce(nonce); }
		PingPl(const PingPl& other) : PingPl_Base(other) { copy(other); }
		PingPl& operator=(const PingPl& other) { if (this == &other) return *this; PingPl_Base::operator=(other); copy(other); return *this; }

		virtual PingPl* dup() const override { return new PingPl(*this); }

		virtual unsigned char* getRawBytes() const override;

		virtual std::string str() const override
		{
			std::stringstream out;
			out << "nonce=" << getNonce();
			std::string baseStr = PingPl_Base::str();
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

template<> inline iblock::bitcoin::payloads::PingPl* fromAnyPtr(any_ptr ptr) { return ptr.get<iblock::bitcoin::payloads::PingPl>(); }

}

#endif
