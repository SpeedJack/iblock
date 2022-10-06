#include "PeerList.h"
#include "omnetpp/ccontextswitcher.h"

namespace iblock
{
namespace bitcoin
{

Define_Module(PeerList);

void PeerList::initialize()
{
}

void PeerList::handleMessage(cMessage *msg)
{
}

void PeerList::addPeer(int gateId)
{
	Enter_Method("addPeer(%d)", gateId);
	if (hasPeer(gateId))
		return;
	Peer *peer = new Peer();
	peer->setGateId(gateId);
	peers.insert(std::pair<int, Peer *>(gateId, peer));
}

bool PeerList::hasPeer(int gateId)
{
	Enter_Method("hasPeer(%d)", gateId);
	return peers.find(gateId) != peers.end();
}

void PeerList::removePeer(int gateId)
{
	Enter_Method("removePeer(%d)", gateId);
	delete peers.at(gateId);
	peers.erase(gateId);
}

void PeerList::removePeer(Peer *peer)
{
	Enter_Method("removePeer(%s)", peer->str().c_str());
	removePeer(peer->getGateId());
}

const Peer *PeerList::getPeer(int gateId)
{
	Enter_Method("getPeer(%d)", gateId);
	return peers.at(gateId);
}

std::vector<const Peer *> PeerList::getAllPeers()
{
	Enter_Method("getAllPeers()");
	std::vector<const Peer *> result;
	for (auto it = peers.begin(); it != peers.end(); it++)
		result.push_back(it->second);
	return result;
}

std::vector<const Peer *> PeerList::getConnectedPeers()
{
	Enter_Method("getConnectedPeers()");
	std::vector<const Peer *> result;
	for (auto it = peers.begin(); it != peers.end(); it++)
		if (it->second->getAcked())
			result.push_back(it->second);
	return result;
}

PeerList::~PeerList()
{
	for (auto it = peers.begin(); it != peers.end(); it++)
		delete it->second;
}

}
}
