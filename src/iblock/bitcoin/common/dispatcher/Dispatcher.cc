#include "Dispatcher.h"
#include "iblock/bitcoin/apps/base/AppBase.h"
#include "iblock/bitcoin/messages/IncomingMessage_m.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Define_Module(Dispatcher);

void Dispatcher::initialize(int stage)
{
	if (stage == 0)
		return;

	peerTable = check_and_cast<PeerTable *>(getModuleByPath(par("peerTableModule").stringValue()));

	if (par("performDiscovery").boolValue())
		for (unsigned int i = 0; i < gateSize("port$i"); ++i)
			peerTable->addPeer(new Peer(gate("port$i", i)));
}

void Dispatcher::handleMessage(cMessage *msg)
{
	if (msg->getArrivalGate()->getBaseId() == gateBaseId("port$i"))
		handleIncomingPacket(check_and_cast<Packet *>(msg));
	else
		handleOutgoingMessage(check_and_cast<OutgoingMessage *>(msg));
}

void Dispatcher::handleIncomingPacket(Packet *pkt)
{
	Peer *peer = peerTable->getPeer(pkt->getArrivalGate());
	if (!peer) {
		EV_WARN << "Can not get sending peer from peer table" << std::endl;
		delete pkt;
		return;
	}

	auto entry = listeners.find(pkt->getKind());
	if (entry == listeners.end()) {
		EV_WARN << "No listener registered for packet kind " << pkt->getKind() << std::endl;
		delete pkt;
		return;
	}

	IncomingMessage *msg = new IncomingMessage(peer, pkt);

	for (auto it = entry->second.begin(); it != entry->second.end(); it++)
		send(std::next(it) == entry->second.end() ? msg : msg->dup(), *it);
}

void Dispatcher::handleOutgoingMessage(OutgoingMessage *msg)
{
	size_t count = msg->getPeerArraySize();
	if (count == 0) {
		EV_WARN << "No peer specified for outgoing message" << std::endl;
		delete msg;
		return;
	}

	Packet *pkt = msg->removeNetworkPacket();
	for (size_t i = 0; i < count; i++)
		send(i == count-1 ? pkt : pkt->dup(), const_cast<cGate *>(msg->getPeer(i)->getGate()->getOtherHalf()));
	delete msg;
}

void Dispatcher::registerListener(AppBase *app, short kind)
{
	Enter_Method("registerListener(%s, %hi)", app->getFullPath().c_str(), kind);

	cGate *appGate = app->gate("in")->getPathStartGate();
	if (!appGate || appGate->getOwnerModule() != this)
		error("AppBase::registerListener(): App %s is not connected to the dispatcher", app->getFullPath().c_str());

	auto entry = listeners.find(kind);
	if (entry == listeners.end()) {
		std::set<cGate *> gates;
		gates.insert(appGate);
		listeners.insert(std::make_pair(kind, gates));
	} else {
		entry->second.insert(appGate);
	}

	EV_INFO << "App " << app->getFullPath() << " registered as listener for message kind " << kind << std::endl;
}

}
}
