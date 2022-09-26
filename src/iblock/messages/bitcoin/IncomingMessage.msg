import iblock.primitives.bitcoin.Peer;
import iblock.messages.bitcoin.Packet;

namespace iblock::bitcoin;

message IncomingMessage
{
	const Peer *peer;
	Packet *payload @owned;
};
