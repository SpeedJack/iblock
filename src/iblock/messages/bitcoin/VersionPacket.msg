import iblock.messages.bitcoin.Packet;

cplusplus(h) {{
#include "Packet.h"
}}

namespace iblock::bitcoin;

packet VersionPacket extends Packet
{
	@customize(true);
	int32_t version = 70015;
	uint64_t services = 0x01;
	simtime_t timestamp;
	uint64_t addrRecvServices = 0x00;
	string addrRecvIp = "::ffff:127.0.0.1";
	uint16_t addrRecvPort = 8333;
	string addrTransIp = "::ffff:127.0.0.1";
	uint16_t addrTransPort = 8333;
	uint64_t nonce = 0;
	string userAgent = "/OMNeT++:6.0.1/INET:4.4.1/iblock:0.9.0/";
	int32_t startHeight = 0;
	bool relay = true;
};
