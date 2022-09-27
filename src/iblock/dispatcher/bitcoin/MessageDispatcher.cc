#include "MessageDispatcher.h"
#include "../../messages/bitcoin/OutgoingMessage_m.h"
#include "../../messages/bitcoin/Packet_m.h"

namespace iblock
{
namespace bitcoin
{

Define_Module(MessageDispatcher);

void MessageDispatcher::initialize()
{
	peerListModule = check_and_cast<PeerList *>(getModuleByPath(par("peerListModule").stringValue()));

	for (unsigned int i = 0; i < gateSize("port$i"); ++i) {
		cGate *gate = this->gate("port$i", i);
		peerListModule->addPeer(gate->getId());
	}
}

IncomingMessage *MessageDispatcher::packMessage(cGate *gate, cPacket *incoming)
{
	Packet *pckt = check_and_cast<Packet *>(incoming);
	IncomingMessage *msg = new IncomingMessage();
	msg->setKind(pckt->getKind());
	msg->setPayload(pckt);
	msg->setPeer(peerListModule->getPeer(gate->getId()));
	return msg;
}

void MessageDispatcher::forwardMessage(cMessage *outgoing)
{
	OutgoingMessage *msg = check_and_cast<OutgoingMessage *>(outgoing);
	Packet *payload = const_cast<Packet *>(msg->removePayload());

	size_t count = msg->getPeerArraySize();
	if (count == 0) { // Broadcast
		unsigned int gateCount = gateSize("port$o");
		for (unsigned int i = 0; i < gateCount; ++i) {
			cGate *gate = this->gate("port$o", i);
			send(i == gateCount - 1 ? payload : payload->dup(), gate);
		}
		return;
	}

	for (size_t i = 0; i < count; ++i) {
		const Peer *peer = msg->getPeer(i);
		cGate *gate = this->gate(peer->getGateId())->getOtherHalf();
		send(i == count - 1 ? payload : payload->dup(), gate);
	}
}

}
}
