import iblock.messages.bitcoin.Packet;

cplusplus(h) {{
#include "Packet.h"
}}

namespace iblock::bitcoin;

packet VerackPacket extends Packet
{
	@customize(true);
};
