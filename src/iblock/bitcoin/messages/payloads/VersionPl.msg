import iblock.bitcoin.messages.payloads.Payload;
import iblock.bitcoin.common.IPAddress;

namespace iblock::bitcoin::payloads;

packet VersionPl extends Payload
{
	@customize;
	@toString(.str());
	int32_t version = 70015;
	uint64_t services = 0x01;
	simtime_t timestamp;
	uint64_t addrRecvServices = 0;
	IPAddress addrRecvIp = "::ffff:127.0.0.1" @byValue;
	uint16_t addrRecvPort = 8333;
	abstract uint64_t addrTransServices;
	IPAddress addrTransIp = "::ffff:127.0.0.1" @byValue;
	uint16_t addrTransPort = 8333;
	uint64_t nonce = 0;
	abstract const unsigned long userAgentBytes;
	string userAgent = "/OMNeT++:6.0.1/INET:4.4.1/iblock:0.9.0";
	int32_t startHeight = 0;
	bool relay = true;
}
