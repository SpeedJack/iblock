#ifndef __IBLOCK_BITCOIN_PAYLOADS_VERSIONPL_H_
#define __IBLOCK_BITCOIN_PAYLOADS_VERSIONPL_H_

#include "VersionPl_m.h"
#include "iblock/bitcoin/messages/MessageKind_m.h"

namespace iblock
{
namespace bitcoin
{
namespace payloads
{

class IBLOCK_API VersionPl : public VersionPl_Base
{
	private:
		void copy(const VersionPl& other);

	protected:
		void updateByteLength();

	public:
		VersionPl() : VersionPl_Base() { setKind(MessageKind::VERSION); setName("version"); setTimestamp(::omnetpp::simTime()); updateByteLength(); }
		VersionPl(const VersionPl& other) : VersionPl_Base(other) { copy(other); }
		VersionPl& operator=(const VersionPl& other) { if (this == &other) return *this; VersionPl_Base::operator=(other); copy(other); return *this; }

		virtual VersionPl* dup() const override { return new VersionPl(*this); }

		virtual uint64_t getAddrTransServices() const override { return getServices(); }
		virtual void setAddrTransServices(uint64_t addrTransServices) override { setServices(addrTransServices); }

		virtual unsigned long getUserAgentBytes() const override { return userAgent.size(); }

		virtual unsigned char* getRawBytes() const override;
		virtual void setVersion(int32_t version) override { VersionPl_Base::setVersion(version); updateByteLength(); }
		virtual void setUserAgent(const char *userAgent) override { VersionPl_Base::setUserAgent(userAgent); updateByteLength(); }

		virtual std::string str() const override;
};

}
}
}

namespace omnetpp
{

template<> inline iblock::bitcoin::payloads::VersionPl* fromAnyPtr(any_ptr ptr) { return ptr.get<iblock::bitcoin::payloads::VersionPl>(); }

}

#endif
