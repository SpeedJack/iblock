import iblock.bitcoin.common.Network;
import iblock.bitcoin.messages.payloads.Payload;

namespace iblock::bitcoin;

packet Packet
{
	@customize;
	@toString(.str());
	Network startString = Network::Mainnet;
	abstract const string commandName;
	abstract const uint32_t payloadSize;
	uint32_t checksum;
	abstract const uint32_t computedChecksum;
}
