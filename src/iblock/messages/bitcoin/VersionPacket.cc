#include "VersionPacket.h"

#include <cstring>
#include <string>

namespace iblock
{
namespace bitcoin
{

Register_Class(VersionPacket);

void VersionPacket::copy(const VersionPacket& other)
{
}

unsigned char *VersionPacket::getRawBytes() const
{
	unsigned char *rawBytes = new unsigned char[getPayloadSize()];
	unsigned char *p = rawBytes;

	std::memcpy(p, &version, sizeof(version));
	p += sizeof(version);
	std::memcpy(p, &services, sizeof(services));
	p += sizeof(services);
	std::memcpy(p, &timestamp, sizeof(timestamp));
	p += sizeof(timestamp);
	std::memcpy(p, &addrRecvServices, sizeof(addrRecvServices));
	p += sizeof(addrRecvServices);
	const char *str = addrRecvIp.c_str();
	for (int i = 0; i < 16; i++)
		*p++ = i < strlen(str) ? str[i] : '\0';
	std::memcpy(p, &addrRecvPort, sizeof(addrRecvPort));
	p += sizeof(addrRecvPort);

	if (version < 106)
		return rawBytes;
	std::memcpy(p, &services, sizeof(services));
	p += sizeof(services);
	str = addrTransIp.c_str();
	for (int i = 0; i < 16; i++)
		*p++ = i < strlen(str) ? str[i] : '\0';
	std::memcpy(p, &addrTransPort, sizeof(addrTransPort));
	p += sizeof(addrTransPort);
	std::memcpy(p, &nonce, sizeof(nonce));
	p += sizeof(nonce);
	int uaBytes = userAgent.size();
	unsigned char compSize[9];
	size_t bytesUsed = compactSize((uint32_t)uaBytes, compSize);
	std::memcpy(p, compSize, bytesUsed);
	p += bytesUsed;
	str = userAgent.c_str();
	for (int i = 0; i < uaBytes; i++)
		*p++ = str[i];

	if (version < 209)
		return rawBytes;
	std::memcpy(p, &startHeight, sizeof(startHeight));
	p += sizeof(startHeight);

	if (version < 70001)
		return rawBytes;
	*p = relay ? (unsigned char)1 : (unsigned char)0;

	return rawBytes;
}

VersionPacket::VersionPacket(uint32_t version, int64_t payloadBytes,
		const char *name, short kind): VersionPacket_Base(name, kind)
{
	this->version = version;
	if (payloadBytes == -1) {
		payloadBytes = 46;
		if (version >= 106) {
			payloadBytes += 34;
			int uaBytes = userAgent.size();
			size_t bytesUsed = compactSize((uint32_t)uaBytes);
			payloadBytes += bytesUsed + uaBytes;
		}
		if (version >= 209)
			payloadBytes += 4;
		if (version >= 70001)
			payloadBytes++;
	}
	setPayloadSize(payloadBytes);
}

void VersionPacket::setUserAgent(const char *userAgent)
{
	VersionPacket_Base::setUserAgent(userAgent);
	int uaBytes = this->userAgent.size();
	int newUaBytes = strlen(userAgent);
	addPayloadSize(compactSize((uint32_t)newUaBytes) + newUaBytes -
			(compactSize((uint32_t)uaBytes) + uaBytes));
}

std::string VersionPacket::str() const
{
	std::stringstream out;
	out << "version=" << version << ", ";
	out << "services=" << services << ", "; //TODO
	out << "timestamp=" << timestamp.format() << ", ";
	out << "addrRecvServices=" << addrRecvServices << ", "; //TODO
	out << "addrRecvIp=" << addrRecvIp << ", ";
	out << "addrRecvPort=" << addrRecvPort << ", ";
	out << "addrTransIp=" << addrTransIp << ", ";
	out << "addrTransPort=" << addrTransPort << ", ";
	out << "nonce=" << nonce << ", ";
	out << "userAgent=" << userAgent << ", ";
	out << "startHeight=" << startHeight << ", ";
	out << "relay=" << (relay ? "true" : "false");

	std::string baseStr = Packet::str();
	if (!baseStr.empty())
		out << "; " << baseStr;
	return out.str();
}

}
}
