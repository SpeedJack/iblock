#include "PeerTable.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Define_Module(PeerTable);

void PeerTable::addPeer(Peer* peer)
{
	Enter_Method("addPeer(..)");
	if (peer == nullptr)
		return;

	if (peers.find({peer->getGateId(), {peer->getAddress(), peer->getPort()}}) != peers.end()) {
		EV_WARN << "Peer " << peer->getId() << " at " << peer->getGateId() << "$[" << peer->getAddress().str() << "]:" << peer->getPort() << " already exists in peer table" << std::endl;
		return;
	}

	peers.insert({{peer->getGateId(), {peer->getAddress(), peer->getPort()}}, peer});
	gateMap.insert({peer->getGateId(), peer});
	IPMap.insert({{peer->getAddress(), peer->getPort()}, peer});

	peer->setId(nextId++);
	peer->setUpdateCallback([this, peer](cGate* gate, IPAddress address, uint16_t port){ this->updatePeer(peer, gate, address, port); });
}

std::vector<Peer*> PeerTable::getAllPeers(bool connected) const
{
	Enter_Method("getAllPeers(%s)", connected ? "true" : "false");
	std::vector<Peer*> result;
	for (auto& peer : peers)
		if (!connected || peer.second->isConnected())
			result.push_back(peer.second);
	return result;
}

std::vector<Peer *> PeerTable::getPeers(Network network, bool connected) const
{
	Enter_Method("getPeers(%x, %s)", network, connected ? "true" : "false");
	std::vector<Peer*> result;
	for (auto& peer : peers)
		if (peer.second->getNetwork() == network && (!connected || peer.second->isConnected()))
			result.push_back(peer.second);
	return result;
}

std::vector<Peer*> PeerTable::getPeers(int gateId, Network network, bool connected) const
{
	Enter_Method("getPeers(%d, %x, %s)", gateId, network, connected ? "true" : "false");
	std::vector<Peer*> result;
	auto range = gateMap.equal_range(gateId);
	for (auto it = range.first; it != range.second; ++it)
		if (it->second->getNetwork() == network && (!connected || it->second->isConnected()))
			result.push_back(it->second);
	return result;
}

Peer* PeerTable::getPeer(int gateId, IPAddress address, uint16_t port) const
{
	Enter_Method("getPeer(%d, %s, %u)", gateId, address.str().c_str(), port);
	try {
		return peers.at({gateId, {address, port}});
	} catch (std::out_of_range& ex) {
		EV_WARN << "Peer " << gateId << "$[" << address.str() << "]:" << port << " does not exist in peer table" << std::endl;
		return nullptr;
	}
}

Peer* PeerTable::getPeer(int gateId) const
{
	Enter_Method("getPeer(%d)", gateId);
	auto it = gateMap.find(gateId);
	if (it == gateMap.end()) {
		EV_WARN << "No peer with gateId=" << gateId << " exists in peer table" << std::endl;
		return nullptr;
	}
	return it->second;
}

Peer* PeerTable::getPeer(IPAddress address, uint16_t port) const
{
	Enter_Method("getPeer(%s, %u)", address.str().c_str(), port);
	auto it = IPMap.find({address, port});
	if (it == IPMap.end()) {
		EV_WARN << "No peer with address=" << address.str() << ",port=" << port << " exists in peer table" << std::endl;
		return nullptr;
	}
	return it->second;
}

Peer* PeerTable::getPeerAtIndex(unsigned int index) const
{
	Enter_Method("getPeerAtIndex(%u)", index);
	for (auto& peer : peers)
		if (index-- == 0)
			return peer.second;
	return nullptr;
}

void PeerTable::removeGateMap(const Peer* peer)
{
	auto range = gateMap.equal_range(peer->getGateId());
	for (auto it = range.first; it != range.second; ++it)
		if (it->second == peer) {
			gateMap.erase(it);
			break;
		}
}

void PeerTable::removeIPMap(const Peer* peer)
{
	auto range = IPMap.equal_range({peer->getAddress(), peer->getPort()});
	for (auto it = range.first; it != range.second; ++it)
		if (it->second == peer) {
			IPMap.erase(it);
			break;
		}
}

Peer* PeerTable::removePeer(Peer* peer)
{
	Enter_Method("removePeer(peerid:%u)", peer->getId());
	if (peer == nullptr)
		return nullptr;

	peers.erase({peer->getGateId(), {peer->getAddress(), peer->getPort()}});
	removeGateMap(peer);
	removeIPMap(peer);

	return peer;
}

bool PeerTable::hasPeer(int gateId) const
{
	Enter_Method("hasPeer(%d)", gateId);
	return gateMap.find(gateId) != gateMap.end();
}

bool PeerTable::hasPeer(IPAddress address, uint16_t port) const
{
	Enter_Method("hasPeer(%s, %hu)", address.str().c_str(), port);
	return IPMap.find({address, port}) != IPMap.end();
}

bool PeerTable::hasPeer(int gateId, IPAddress address, uint16_t port) const
{
	Enter_Method("hasPeer(%d, %s, %hu)", gateId, address.str().c_str(), port);
	return peers.find({gateId, {address, port}}) != peers.end();
}

PeerTable::~PeerTable()
{
	for (auto& peer : peers)
		delete peer.second;
}

void PeerTable::updatePeer(Peer* peer, int gateId, IPAddress address, uint16_t port)
{
	if (peer == nullptr || (peer->getGateId() == gateId && peer->getAddress() == address && peer->getPort() == port))
		return;

	peers.erase({peer->getGateId(), {peer->getAddress(), peer->getPort()}});
	peers.insert({{peer->getGateId(), {peer->getAddress(), peer->getPort()}}, peer});

	if (peer->getGateId() != gateId) {
		removeGateMap(peer);
		gateMap.insert({peer->getGateId(), peer});
	}
	if (peer->getAddress() != address || peer->getPort() != port) {
		removeIPMap(peer);
		IPMap.insert({{peer->getAddress(), peer->getPort()}, peer});
	}
}

}
}
