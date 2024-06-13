#include "Pinger.h"

using namespace omnetpp;
using namespace iblock::bitcoin::payloads;

namespace iblock
{
namespace bitcoin
{

Define_Module(Pinger)

void Pinger::initialize()
{
	AppBase::initialize();

	// Register with dispatcher
	dispatcher->registerListener(this, MessageKind::PING);
	dispatcher->registerListener(this, MessageKind::PONG);

	// Timer
	timer = new cMessage("timer");
	scheduleAfter(par("pingInterval").doubleValue(), timer);
}

void Pinger::handlePingPacket(Peer *peer, PingPl *ping) // AppBase calls this when ping arrives
{
	// Send pong, same nonce
	sendPacket(new PongPl(ping->getNonce()), peer);
	delete ping;
}

void Pinger::handlePongPacket(Peer *peer, PongPl *pong) // AppBase calls this when pong arrives
{
	delete pong; // Should also check nonce
}

void Pinger::handleSelfMessage(cMessage *msg) // timer elapsed
{
	// Select a random peer from PeerTable
	unsigned int index = intuniform(0, peerTable->getPeerCount() - 1);
	Peer *peer = peerTable->getPeerAtIndex(index);
	// Send ping, nonce = 1234
	sendPacket(new PingPl(1234), peer);
	// reschedule timer
	scheduleAfter(par("pingInterval").doubleValue(), timer);
}

}
}
