#include "Packet.h"
#include "MessageKind_m.h"

#include <openssl/sha.h>
#include <iomanip>

using namespace omnetpp;
using iblock::bitcoin::payloads::Payload;

namespace iblock
{
namespace bitcoin
{

Register_Class(Packet);

void Packet::copy(const Packet& other)
{
	startString = other.startString;
	checksum = other.checksum;
	computedChecksum = other.computedChecksum;
}

unsigned char *Packet::sha256hash(const unsigned char *input, size_t len)
{
	unsigned char *hash = new unsigned char[SHA256_DIGEST_LENGTH];

	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, input, len);
	SHA256_Final(hash, &ctx);

	return hash;
}

uint32_t Packet::computeChecksum()
{
	if (computedChecksum != 0)
		return computedChecksum;

	uint32_t payloadSize = getPayloadSize();
	if (payloadSize == 0) {
		computedChecksum = 0x5df6e0e2;
		return computedChecksum;
	}

	Payload *payload = getPayload();
	if (payload == nullptr)
		throw cRuntimeError("There is no payload but payloadSize is greater than zero");

	unsigned char *rawBytes = getPayload()->getRawBytes();
	if (rawBytes == nullptr)
		throw cRuntimeError("Payload is empty but payloadSize is greater than zero");

	unsigned char *first = sha256hash(rawBytes, payloadSize);
	delete[] rawBytes;
	unsigned char *second = sha256hash(first, SHA256_DIGEST_LENGTH);
	delete[] first;
	computedChecksum = *(uint32_t *)second;
	delete[] second;

	return computedChecksum;
}

#define ENUM_NAME(type)			opp_typename(typeid(type))
#define ENUM_ITEM_NAME(type, value)	cEnum::get(ENUM_NAME(type))->getStringFor(value)

void Packet::encapsulate(cPacket *packet)
{
	Payload *payload = dynamic_cast<Payload *>(packet);
	if (!payload)
		throw cRuntimeError("Packet class can encapsulate only Payload packets");

	short msgKind = payload->getKind();
	std::string kindName = opp_strlower(ENUM_ITEM_NAME(MessageKind, msgKind));
	setName(kindName.c_str());
	setKind(msgKind);
	Packet_Base::encapsulate(packet);
}

Payload *Packet::decapsulate()
{
	setName("<empty-Packet>");
	setKind(0);
	return static_cast<Payload *>(Packet_Base::decapsulate());
}

std::string Packet::str() const
{
	std::stringstream out;
	out << "start_string=0x" << std::hex << std::setfill('0') << std::setw(8) << getStartString() << ", ";
	out << "command_name=" << getCommandName() << ", ";
	out << "payload_size=" << getPayloadSize() << ", ";
	out << "checksum=" << getChecksum();
	uint32_t cc = getComputedChecksum();
	if (cc == getChecksum())
		out << " (valid)";
	else
		out << " (invalid: should be " << cc << ")";
	std::string baseStr = Packet_Base::str();
	if (!baseStr.empty())
		out << "; " << baseStr;
	return out.str();
}

}
}
