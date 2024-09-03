#ifndef __IBLOCK_BITCOIN_PAYLOADS_PAYLOAD_H_
#define __IBLOCK_BITCOIN_PAYLOADS_PAYLOAD_H_

#include "iblock/bitcoin/bitcoin.h"

namespace iblock
{
namespace bitcoin
{
namespace payloads
{

class IBLOCK_API Payload : public ::omnetpp::cPacket
{
	private:
		void copy(const Payload& other) { }

	public:
		Payload(const char* name = nullptr, short kind = 0) : cPacket(name, kind) { }
		Payload(const Payload& other) : cPacket(other) { }
		Payload& operator=(const Payload& other) { if (this == &other) return *this; cPacket::operator=(other); copy(other); return *this; }
		virtual Payload* dup() const override = 0;

		virtual unsigned char* getRawBytes() const = 0;
		std::string getRawBytesHexStr() const;
};

}
}
}

#endif
