#include "AppBase.h"

using namespace omnetpp;
using namespace iblock::bitcoin::payloads;

namespace iblock
{
namespace bitcoin
{

void AppBase::initialize()
{
	peerTable = check_and_cast<PeerTable *>(getModuleByPath(par("peerTableModule").stringValue()));
	dispatcher = check_and_cast<Dispatcher *>(getModuleByPath(par("dispatcherModule").stringValue()));
	connectionManager = check_and_cast<IConnectionManager *>(getModuleByPath(par("connectionManagerModule").stringValue()));
	queue.clear();
}

void AppBase::handleMessage(cMessage *msg)
{
	if (msg->isSelfMessage())
		handleSelfMessage(msg);
	else if (msg->arrivedOn("in"))
		handleIncomingMessage(check_and_cast<IncomingMessage *>(msg));
	else
		handleOtherMessage(msg);
}

void AppBase::handleSelfMessage(cMessage *msg)
{
	EV_WARN << "Self message " << msg << " not handled by " << getFullPath() << endl;
	delete msg;
}

void AppBase::handleIncomingMessage(IncomingMessage *msg)
{
	Peer *peer = getPeer(msg);
	Packet *pkt = msg->removeNetworkPacket();
	switch (pkt->getKind()) {
		case MessageKind::PING:
			handlePingPacket(peer, pkt);
			break;
		case MessageKind::PONG:
			handlePongPacket(peer, pkt);
			break;
		case MessageKind::VERSION:
			handleVersionPacket(peer, pkt);
			break;
		case MessageKind::VERACK:
			handleVerackPacket(peer, pkt);
			break;
		default:
			handleUnknownPacket(peer, pkt);
	}
	delete pkt;
	delete msg;
}

void AppBase::handleOtherMessage(cMessage *msg)
{
	EV_WARN << "Message " << msg << " not handled by " << getFullPath() << endl;
	delete msg;
}

void AppBase::sendPacket(OutgoingMessage *msg, bool connect)
{
	if (!connect) {
		send(msg, "out");
		return;
	}

	if (msg->getPeerArraySize() == 0) {
		std::vector<Peer *> peers = peerTable->getAllPeers();
		for (Peer *peer : peers)
			msg->appendPeer(peer);
	}

	bool allConnected = true;
	msgid_t msgid = msg->getId();
	for (size_t i = 0; i < msg->getPeerArraySize(); ++i) {
		const Peer *constPeer = check_and_cast<const Peer *>(msg->getPeer(i));
		if (!constPeer->isConnected()) {
			allConnected = false;
			Peer *peer = const_cast<Peer *>(constPeer);
			connectionManager->connect(peer, [this, msgid](bool success){ this->onPeerConnected(success, msgid); });
		}
	}

	if (allConnected)
		send(msg, "out");
	else
		queue[msgid] = msg;
}

void AppBase::sendPacket(Packet *packet, std::initializer_list<Peer *> peers, bool connect, bool waitAll)
{
	OutgoingMessage *msg = new OutgoingMessage(packet);
	if (!connect || waitAll) {
		for (Peer *peer : peers)
			msg->appendPeer(peer);
		sendPacket(msg, connect);
		return;
	}

	if (peers.size() > 0) {
		for (Peer *peer : peers)
			if (peer->isConnected())
				msg->appendPeer(peer);
			else
				sendPacket(new OutgoingMessage(peer, packet));
		send(msg, "out");
		return;
	}

	std::vector<Peer *> allPeers = peerTable->getAllPeers();
	for (Peer *peer : allPeers)
		if (peer->isConnected())
			msg->appendPeer(peer);
		else
			sendPacket(new OutgoingMessage(peer, packet));
	send(msg, "out");
}

void AppBase::onPeerConnected(bool success, msgid_t msgid)
{
	if (!success)
		return; // TODO: handle

	auto it = queue.find(msgid);
	if (it == queue.end())
		return; // TODO: handle

	OutgoingMessage *msg = it->second;
	for (size_t i = msg->getPeerArraySize(); i > 0; --i) {
		const Peer *peer = check_and_cast<const Peer *>(msg->getPeer(i - 1));
		if (!peer->isConnected())
			return;
	}

	queue.erase(it);
	send(msg, "out");
}

AppBase::~AppBase()
{
	for (auto it = queue.begin(); it != queue.end(); ++it)
		delete it->second;
}

}
}
