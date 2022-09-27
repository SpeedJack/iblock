#include "Control.h"
#include "../../messages/bitcoin/IncomingMessage_m.h"
#include "../../messages/bitcoin/OutgoingMessage_m.h"
#include "../../messages/bitcoin/MessageKind_m.h"
#include "../../messages/bitcoin/VersionPacket_m.h"
#include "../../messages/bitcoin/VerackPacket_m.h"

namespace iblock
{
namespace bitcoin
{

Define_Module(Control);

void Control::initialize()
{
	peerListModule = check_and_cast<PeerList *>(getModuleByPath(par("peerListModule").stringValue()));
	dispatcherModule = check_and_cast<MessageDispatcher *>(getModuleByPath(par("dispatcherModule").stringValue()));

	dispatcherModule->registerListener(this, MessageKind::VERSION);
	dispatcherModule->registerListener(this, MessageKind::VERACK);

	scheduleAt(simTime() + par("initialDelay").doubleValue(), new cMessage("initialDelay"));
}

void Control::broadcastVersion()
{
	VersionPacket *pckt = new VersionPacket();
	pckt->setKind(MessageKind::VERSION);
	pckt->setVersion(70015);
	pckt->setTimestamp(simTime());

	OutgoingMessage *msg = new OutgoingMessage();
	msg->setPayload(pckt);

	send(msg, "out");
}

void Control::handleMessage(cMessage *msg)
{
	if (msg->isSelfMessage()) {
		broadcastVersion();
		delete msg;
		return;
	}
	IncomingMessage *incoming = check_and_cast<IncomingMessage *>(msg);
	const Peer *peer = incoming->getPeer();
	const Packet *payload = incoming->getPayload();
	MessageKind kind = (MessageKind)payload->getKind();
	switch (kind) {
	case MessageKind::VERSION:
		handleVersionMessage(peer, payload);
		break;
	case MessageKind::VERACK:
		handleVerackMessage(peer, payload);
		break;
	default:
		delete incoming;
		throw cRuntimeError("Unknown message kind: %d", kind);
	}
	delete incoming;
}

void Control::handleVersionMessage(const Peer *peer, const Packet *payload)
{
	const VersionPacket *version = check_and_cast<const VersionPacket *>(payload);
	Peer *p = peerListModule->getPeerForUpdate(peer->getGateId());
	p->setVersion(version->getVersion());
	p->setTimestamp(version->getTimestamp());
	Packet *pckt = new Packet();
	pckt->setKind(MessageKind::VERACK);
	OutgoingMessage *msg = new OutgoingMessage();
	msg->appendPeer(peer);
	msg->setPayload(pckt);
	send(msg, "out");
}

void Control::handleVerackMessage(const Peer *peer, const Packet *payload)
{
	const VerackPacket *verack = check_and_cast<const VerackPacket *>(payload);
	Peer *p = peerListModule->getPeerForUpdate(peer->getGateId());
	p->setAcked(true);
}

}
}
