#include "VersionPl.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{
namespace payloads
{

Register_Class(VersionPl)

void VersionPl::copy(const VersionPl& other)
{
	version = other.version;
	services = other.services;
	timestamp = other.timestamp;
	addrRecvServices = other.addrRecvServices;
	addrRecvIp = other.addrRecvIp;
	addrRecvPort = other.addrRecvPort;
	addrTransIp = other.addrTransIp;
	addrTransPort = other.addrTransPort;
	nonce = other.nonce;
	userAgent = other.userAgent;
	startHeight = other.startHeight;
	relay = other.relay;
}

void VersionPl::updateByteLength()
{
	setByteLength(4 + 8 + 8 + 8 + 16 + 2);
	if (version < 106)
		return;
	addByteLength(8 + 16 + 2 + 8);
	unsigned long uaBytes = getUserAgentBytes();
	size_t bytesUsed = compactSize(uaBytes);
	addByteLength(bytesUsed + uaBytes);
	if (version < 209)
		return;
	addByteLength(4);
	if (version < 70001)
		return;
	addByteLength(1);
}

unsigned char *VersionPl::getRawBytes() const
{
	unsigned char *rawBytes = new unsigned char[getByteLength()];
	unsigned char *ptr = rawBytes;

	std::memcpy(ptr, &version, sizeof(version));
	ptr += sizeof(version);
	std::memcpy(ptr, &services, sizeof(services));
	ptr += sizeof(services);
	int64_t ts = getTimestamp().inUnit(SimTimeUnit::SIMTIME_S);
	std::memcpy(ptr, &ts, sizeof(ts));
	ptr += sizeof(ts);
	std::memcpy(ptr, &addrRecvServices, sizeof(addrRecvServices));
	ptr += sizeof(addrRecvServices);
	uint64_t hi = __builtin_bswap64(getAddrRecvIp().getHi());
	uint64_t lo = __builtin_bswap64(getAddrRecvIp().getLo());
	std::memcpy(ptr, &hi, sizeof(hi));
	ptr += sizeof(hi);
	std::memcpy(ptr, &lo, sizeof(lo));
	ptr += sizeof(lo);
	uint16_t port = __builtin_bswap16(addrRecvPort);
	std::memcpy(ptr, &port, sizeof(port));
	ptr += sizeof(port);

	if (version < 106)
		return rawBytes;
	std::memcpy(ptr, &services, sizeof(services));
	ptr += sizeof(services);
	hi = __builtin_bswap64(getAddrTransIp().getHi());
	lo = __builtin_bswap64(getAddrTransIp().getLo());
	std::memcpy(ptr, &hi, sizeof(hi));
	ptr += sizeof(hi);
	std::memcpy(ptr, &lo, sizeof(lo));
	ptr += sizeof(lo);
	port = __builtin_bswap16(addrTransPort);
	std::memcpy(ptr, &port, sizeof(port));
	ptr += sizeof(port);
	std::memcpy(ptr, &nonce, sizeof(nonce));
	ptr += sizeof(nonce);
	unsigned long uaBytes = getUserAgentBytes();
	unsigned char compSize[9];
	size_t bytesUsed = compactSize(uaBytes, compSize);
	std::memcpy(ptr, compSize, bytesUsed);
	ptr += bytesUsed;
	if (uaBytes > 0) {
		std::strncpy((char *)ptr, userAgent.c_str(), uaBytes);
		ptr += uaBytes;
	}

	if (version < 209)
		return rawBytes;
	std::memcpy(ptr, &startHeight, sizeof(startHeight));
	ptr += sizeof(startHeight);

	if (version < 70001)
		return rawBytes;
	*ptr = relay ? (unsigned char) 1 : (unsigned char) 0;

	return rawBytes;
}

std::string VersionPl::str() const
{
	std::stringstream out;
	out << "version=" << getVersion() << ", ";
	out << "services=" << getServices() << ", ";
	out << "timestamp=" << getTimestamp() << ", ";
	out << "addrRecvServices=" << getAddrRecvServices() << ", ";
	out << "addrRecvIp=" << getAddrRecvIp().str() << ", ";
	out << "addrRecvPort=" << getAddrRecvPort() << ", ";
	out << "addrTransIp=" << getAddrTransIp().str() << ", ";
	out << "addrTransPort=" << getAddrTransPort() << ", ";
	out << "nonce=" << getNonce() << ", ";
	out << "userAgentBytes=" << getUserAgentBytes() << ", ";
	out << "userAgent=" << getUserAgent();

	if (version >= 209)
		out << ", startHeight=" << getStartHeight();
	if (version >= 70001)
		out << ", relay=" << (getRelay() ? "True" : "False");

	std::string baseStr = VersionPl_Base::str();
	if (!baseStr.empty())
		out << "; " << baseStr;
	return out.str();
}

}
}
}
