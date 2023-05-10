#ifndef __IBLOCK_BITCOIN_PAYLOADS_PAYLOAD_H_
#define __IBLOCK_BITCOIN_PAYLOADS_PAYLOAD_H_

#include <omnetpp.h>

namespace iblock
{
namespace bitcoin
{
namespace payloads
{

class Payload : public ::omnetpp::cPacket
{
	private:
		void copy(const Payload& other) { }

	protected:
		static size_t compactSize(unsigned long value, unsigned char *result);
		static unsigned long compactSizeValue(const unsigned char *result);

	public:
		Payload(const char *name = nullptr, short kind = 0) : cPacket(name, kind) { }
		Payload(const Payload& other) : cPacket(other) { }
		Payload& operator=(const Payload& other) { if (this == &other) return *this; cPacket::operator=(other); copy(other); return *this; }
		virtual Payload *dup() const override = 0;

		virtual unsigned char *getRawBytes() const = 0;
};

}
}
}

#endif
