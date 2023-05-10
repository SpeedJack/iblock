import iblock.bitcoin.common.Peer;
import iblock.bitcoin.messages.Packet;

cplusplus {{
	#include "Packet.h"
}}

namespace iblock::bitcoin;

message OutgoingMessage
{
	const Peer *peer[];
	Packet *networkPacket @owned;
}

cplusplus(OutgoingMessage) {{
	public:
		OutgoingMessage(Packet *networkPacket) : cMessage() { setNetworkPacket(networkPacket); }
		OutgoingMessage(const Peer *peer, Packet *networkPacket) : OutgoingMessage(networkPacket) { appendPeer(peer); }
}}

cplusplus(OutgoingMessage::setNetworkPacket) {{
	if (networkPacket != nullptr && this->networkPacket == nullptr) {
		const char *pktname = networkPacket->getName();
		char *name = new char[strlen(pktname)+17];
		strcpy(name, "OutgoingMessage:");
		strcat(name, pktname);
		setName(name);
		setKind(networkPacket->getKind());
	}
}}

cplusplus(OutgoingMessage::removeNetworkPacket) {{
	setName("OutgoingMessage:<empty>");
	setKind(0);
}}
