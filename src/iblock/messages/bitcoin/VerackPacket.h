#ifndef __IBLOCK_BITCOIN_VERACKPACKET_H_
#define __IBLOCK_BITCOIN_VERACKPACKET_H_

#include "VerackPacket_m.h"
#include "MessageKind_m.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

class INET_API VerackPacket: public VerackPacket_Base
{
	private:
		void copy(const VerackPacket& other);
	protected:
		virtual unsigned char *getRawBytes() const override;
	public:
		explicit VerackPacket(const char *name = nullptr, short kind = MessageKind::VERSION): VerackPacket_Base(name, kind) {};
		VerackPacket(const VerackPacket& other): VerackPacket_Base(other) { copy(other); };
		VerackPacket& operator=(const VerackPacket& other) { if (this == &other) return *this; VerackPacket_Base::operator=(other); copy(other); return *this; };
		virtual VerackPacket *dup() const override { return new VerackPacket(*this); };

		virtual std::string str() const override;
};

}
}

#endif
