import iblock.bitcoin.common.Peer;
import iblock.bitcoin.messages.Packet;

cplusplus {{
	#include "Packet.h"
}}

namespace iblock::bitcoin;

message IncomingMessage
{
	const Peer* peer;
	Packet* networkPacket @owned;
}

cplusplus(IncomingMessage) {{
	public:
		IncomingMessage(Peer* peer, Packet* networkPacket) : cMessage() { setPeer(peer); setNetworkPacket(networkPacket); }
}}

cplusplus(IncomingMessage::setNetworkPacket) {{
	if (networkPacket != nullptr && this->networkPacket == nullptr) {
		const char* pktname = networkPacket->getName();
		char* name = new char[strlen(pktname)+17];
		strcpy(name, "IncomingMessage:");
		strcat(name, pktname);
		setName(name);
		setKind(networkPacket->getKind());
	}
}}

cplusplus(IncomingMessage::removeNetworkPacket) {{
	setName("IncomingMessage:<empty>");
	setKind(0);
}}
