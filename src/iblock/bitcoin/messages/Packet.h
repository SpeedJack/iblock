#ifndef __IBLOCK_BITCOIN_PACKET_H_
#define __IBLOCK_BITCOIN_PACKET_H_

#include "Packet_m.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API Packet : public Packet_Base
{
	private:
		void copy(const Packet& other);

	protected:
		uint32_t computedChecksum = 0;

		static unsigned char* sha256hash(const unsigned char* input, size_t len);

	public:
		Packet(const char* commandName, short kind = 0) : Packet_Base(commandName, kind) { setByteLength(24); }
		Packet(Network network = Network::Mainnet, const char* commandName = nullptr) : Packet(commandName, 0) { this->startString = network; }
		Packet(payloads::Payload* payload, Network network = Network::Mainnet) : Packet(network) { encapsulate(payload); }
		Packet(const Packet& other) : Packet_Base(other) { copy(other); }
		Packet& operator=(const Packet& other) { if (this == &other) return *this; Packet_Base::operator=(other); copy(other); return *this; }
		virtual Packet* dup() const override { return new Packet(*this); }

		virtual Network getNetwork() const { return getStartString(); }
		virtual void setNetwork(Network network) { setStartString(network); }

		virtual const char* getCommandName() const override { return getName(); }
		virtual uint32_t getPayloadSize() const override { return getByteLength() - 24; }
		virtual uint32_t getComputedChecksum() const override { return computedChecksum; }
		virtual uint32_t computeChecksum();

		virtual bool isChecksumValid() { return getChecksum() == computeChecksum(); }
		virtual void setChecksumValid(bool valid = true) { setChecksum(computeChecksum()); if (!valid) setChecksum(~getComputedChecksum()); }
		virtual void setChecksumInvalid() { setChecksumValid(false); }

		virtual void encapsulate(cPacket* packet) override;
		virtual payloads::Payload* decapsulate() override;
		virtual payloads::Payload* getEncapsulatedPacket() const override { return static_cast<payloads::Payload*>(Packet_Base::getEncapsulatedPacket()); }
		virtual payloads::Payload* getPayload() const { return getEncapsulatedPacket(); }

		virtual std::string str() const override;
};

}
}

namespace omnetpp
{

template<> inline iblock::bitcoin::Packet* fromAnyPtr(any_ptr ptr) { return ptr.get<iblock::bitcoin::Packet>(); }

}

#endif
