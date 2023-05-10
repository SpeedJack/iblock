#include "Pinger.h"

using namespace omnetpp;
using namespace iblock::bitcoin::payloads;

namespace iblock
{
namespace bitcoin
{

Define_Module(Pinger);

void Pinger::initialize()
{
	AppBase::initialize();

	dispatcher->registerListener(this, MessageKind::PING);
	dispatcher->registerListener(this, MessageKind::PONG);

	timer = new cMessage("timer");
	scheduleAfter(par("pingInterval").doubleValue(), timer);
}

void Pinger::handlePingPacket(Peer *peer, PingPl *ping)
{
	sendPacket(new PongPl(ping->getNonce()), peer);
	delete ping;
}

void Pinger::handlePongPacket(Peer *peer, PongPl *pong)
{
	delete pong;
}

void Pinger::handleSelfMessage(cMessage *msg)
{
	unsigned int index = intuniform(0, peerTable->getPeerCount() - 1);
	Peer *peer = peerTable->getPeerAtIndex(index);
	sendPacket(new PingPl(1234), peer);
	scheduleAfter(par("pingInterval").doubleValue(), timer);
}

}
}
