import iblock.messages.bitcoin.Packet;

namespace iblock::bitcoin;

packet VersionPacket extends Packet
{
	int32_t version;
	simtime_t timestamp;
};
