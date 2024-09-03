#include "ConnectionManager.h"

using namespace omnetpp;
using namespace iblock::bitcoin::payloads;

namespace iblock
{
namespace bitcoin
{

Define_Module(ConnectionManager);

void ConnectionManager::initialize()
{
	AppBase::initialize();
	notifyMap.clear();

	dispatcher->registerListener(this, MessageKind::VERSION);
	dispatcher->registerListener(this, MessageKind::VERACK);
}

void ConnectionManager::handleVersionPacket(Peer* peer, VersionPl* version)
{
	peer->setVersion(version->getVersion());
	peer->setServices(version->getServices());
	peer->setAddress(version->getAddrTransIp());
	peer->setPort(version->getAddrTransPort());
	peer->setUserAgent(version->getUserAgent());
	peer->setKnownHeight(version->getStartHeight());
	peer->setRelay(version->getRelay());

	if (!peer->isVersionSent())
		startHandshake(peer);

	completeHandshake(peer);

	delete version;
}

void ConnectionManager::handleVerackPacket(Peer* peer, VerackPl* verack)
{
	peer->setConnected();
	notifyApps(peer);

	delete verack;
}

void ConnectionManager::connect(Peer* peer, std::function<void(bool)> callback)
{
	Enter_Method("connect(peerid:%d, <callback>)", peer->getId());
	if (peer->isConnected()) {
		callback(true);
		return;
	}

	auto it = notifyMap.find(peer->getId());
	if (it != notifyMap.end())
		it->second.push_back(callback);
	else
		notifyMap.insert(std::make_pair(peer->getId(), std::vector<std::function<void(bool)>>{callback}));

	if (!peer->isVersionSent())
		startHandshake(peer);
}

void ConnectionManager::startHandshake(Peer* peer)
{
	VersionPl* version = new VersionPl();
	version->setAddrRecvServices(peer->getServices());
	version->setAddrRecvIp(peer->getAddress());
	version->setAddrRecvPort(peer->getPort());

	sendPacket(version, peer, false);
	peer->setVersionSent();
}

void ConnectionManager::completeHandshake(Peer* peer)
{
	VerackPl* verack = new VerackPl();
	sendPacket(verack, peer, false);
}

void ConnectionManager::notifyApps(unsigned int peerId)
{
	auto it = notifyMap.find(peerId);
	if (it == notifyMap.end())
		return;

	auto callbacks = it->second;
	for (auto callback : callbacks)
		callback(true);
}

}
}
