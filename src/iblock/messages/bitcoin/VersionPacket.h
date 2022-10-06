#ifndef __IBLOCK_BITCOIN_VERSIONPACKET_H_
#define __IBLOCK_BITCOIN_VERSIONPACKET_H_

#include "VersionPacket_m.h"
#include "MessageKind_m.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

class INET_API VersionPacket: public VersionPacket_Base
{
	private:
		void copy(const VersionPacket& other);
	protected:
		virtual unsigned char *getRawBytes() const override;
	public:
		explicit VersionPacket(uint32_t version = 70015, int64_t payloadBytes = -1, const char *name = nullptr, short kind = MessageKind::VERSION);
		explicit VersionPacket(const char *name, short kind = MessageKind::VERSION): VersionPacket(70015, -1, name, kind) {};
		VersionPacket(const VersionPacket& other): VersionPacket_Base(other) { copy(other); };
		VersionPacket& operator=(const VersionPacket& other) { if (this == &other) return *this; VersionPacket_Base::operator=(other); copy(other); return *this; };
		virtual VersionPacket *dup() const override { return new VersionPacket(*this); };

		virtual void setUserAgent(const char *userAgent) override;

		virtual std::string str() const override;

		virtual uint64_t getAddrTransServices() const { return getServices(); };
};

}
}

#endif
