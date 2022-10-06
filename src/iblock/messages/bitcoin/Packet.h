#ifndef __IBLOCK_BITCOIN_PACKET_H_
#define __IBLOCK_BITCOIN_PACKET_H_

#include "Packet_m.h"
#include "MessageKind_m.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

class INET_API Packet: public Packet_Base
{
	private:
		void copy(const Packet& other);
	protected:
		uint32_t computedChecksum = 0;

		static unsigned char *sha256hash(const unsigned char *input, size_t len);
		virtual unsigned char *getRawBytes() const = 0;
		unsigned char *getFullRawBytes() const;

		static size_t compactSize(uint64_t value, unsigned char const *result = nullptr);

		void setPayloadSize(int64_t size) { setByteLength(size + 24); };
		void addPayloadSize(int64_t delta) { addByteLength(delta); };
	public:
		explicit Packet(const char *name = nullptr, short kind = 0): Packet_Base(name, kind) {};
		explicit Packet(MessageKind kind): Packet(nullptr, kind) {};
		Packet(const Packet& other): Packet_Base(other) { copy(other); };
		Packet& operator=(const Packet& other) { if (this == &other) return *this; Packet_Base::operator=(other); copy(other); return *this; };
		virtual Packet *dup() const override { return new Packet(*this); };

		virtual const char *getCommandName() const override;
		virtual uint32_t getPayloadSize() const override;
		virtual uint32_t getComputedChecksum() const override;

		virtual uint32_t computeChecksum();

		virtual std::string str() const override;
};

}
}

#endif
