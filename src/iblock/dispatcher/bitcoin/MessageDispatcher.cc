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

	for (int i = 0; i < gateSize("port$i"); ++i) {
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
	size_t count = msg->getPeerArraySize();
	for (size_t i = 0; i < count; ++i) {
		const Packet *payload = msg->getPayload();
		const Peer *peer = msg->getPeer(i);
		cGate *gate = this->gate(peer->getGateId())->getOtherHalf();
		send(payload->dup(), gate);
	}
}

}
}
