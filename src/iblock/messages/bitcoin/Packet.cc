#include "Packet.h"
#include "MessageKind_m.h"

#include <openssl/sha.h>

namespace iblock
{
namespace bitcoin
{

void Packet::copy(const Packet& other)
{
	startString = other.startString;
	checksum = other.checksum;
	computedChecksum = other.computedChecksum;
}

unsigned char *Packet::sha256hash(const unsigned char *input, size_t len)
{
	unsigned char *output = new unsigned char[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, input, len);
	SHA256_Final(output, &sha256);
	return output;
}

unsigned char *Packet::getFullRawBytes() const
{
	unsigned char *rawBytes = new unsigned char[getByteLength()];
	unsigned char *p = rawBytes;
	switch (getStartString()) {
	case Network::Mainnet:
		*p++ = 0xf9; *p++ = 0xbe; *p++ = 0xb4; *p++ = 0xd9;
		break;
	case Network::Testnet:
		*p++ = 0x0b; *p++ = 0x11; *p++ = 0x09; *p++ = 0x07;
		break;
	case Network::Regtest:
		*p++ = 0xfa; *p++ = 0xbf; *p++ = 0xb5; *p++ = 0xda;
		break;
	}

	const char *cmd = getCommandName();
	for (int i = 0; i < 12; i++)
		*p++ = i < strlen(cmd) ? cmd[i] : '\0';

	uint32_t buf = getPayloadSize();
	*p++ = (buf >> 0) & 0xff;
	*p++ = (buf >> 8) & 0xff;
	*p++ = (buf >> 16) & 0xff;
	*p++ = (buf >> 24) & 0xff;
	buf = getChecksum();
	*p++ = (buf >> 0) & 0xff;
	*p++ = (buf >> 8) & 0xff;
	*p++ = (buf >> 16) & 0xff;
	*p++ = (buf >> 24) & 0xff;

	unsigned char *payload = getRawBytes();
	std::memcpy(p, &payload, getPayloadSize());
	delete[] payload;

	return rawBytes;
}

const char *Packet::getCommandName() const
{
	switch (getKind()) {
	case MessageKind::BLOCK:
		return "block";
	case MessageKind::GETBLOCKS:
		return "getblocks";
	case MessageKind::GETDATA:
		return "getdata";
	case MessageKind::GETHEADERS:
		return "getheaders";
	case MessageKind::HEADERS:
		return "headers";
	case MessageKind::INV:
		return "inv";
	case MessageKind::MEMPOOL:
		return "mempool";
	case MessageKind::MERKLEBLOCK:
		return "merkleblock";
	case MessageKind::CMPCTBLOCK:
		return "cmpctblock";
	case MessageKind::SENDCMPCT:
		return "sendcmpct";
	case MessageKind::GETBLOCKTXN:
		return "getblocktxn";
	case MessageKind::BLOCKTXN:
		return "blocktxn";
	case MessageKind::NOTFOUND:
		return "notfound";
	case MessageKind::TX:
		return "tx";
	case MessageKind::ADDR:
		return "addr";
	case MessageKind::ADDRV2:
		return "addrv2";
	case MessageKind::ALERT:
		return "alert";
	case MessageKind::FEEFILTER:
		return "feefilter";
	case MessageKind::FILTERADD:
		return "filteradd";
	case MessageKind::FILTERCLEAR:
		return "filterclear";
	case MessageKind::FILTERLOAD:
		return "filterload";
	case MessageKind::GETADDR:
		return "getaddr";
	case MessageKind::PING:
		return "ping";
	case MessageKind::PONG:
		return "pong";
	case MessageKind::REJECT:
		return "reject";
	case MessageKind::SENDHEADERS:
		return "sendheaders";
	case MessageKind::SENDADDRV2:
		return "sendaddrv2";
	case MessageKind::VERACK:
		return "verack";
	case MessageKind::VERSION:
		return "version";
	default:
		return "unknown";
	}
}

uint32_t Packet::getPayloadSize() const
{
	return getByteLength() - 24;
}

uint32_t Packet::getComputedChecksum() const
{
	return computedChecksum;
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

	unsigned char *rawBytes = getRawBytes();
	if (rawBytes == nullptr) {
		computedChecksum = 0x5df6e0e2;
		return computedChecksum;
	}

	unsigned char *first = sha256hash(rawBytes, payloadSize);
	delete[] rawBytes;
	unsigned char *second = sha256hash(first, SHA256_DIGEST_LENGTH);
	delete[] first;
	computedChecksum = (second[0] << 0) | (second[1] << 8) | (second[2] << 16) | (second[3] << 24);
	delete[] second;
	return computedChecksum;
}

std::string Packet::str() const
{
	std::stringstream out;
	out << "start_string=" << getStartString() << ", ";
	out << "command_name=" << getCommandName() << ", ";
	out << "payload_size=" << getPayloadSize() << ", ";
	out << "checksum=" << checksum << " (real=" << computedChecksum << ")";
	std::string baseStr = Packet_Base::str();
	if (!baseStr.empty())
		out << "; " << baseStr;
	return out.str();
}

}
}
