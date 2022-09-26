#include "PeerListBase.h"

namespace iblock
{

template<typename T>
void PeerListBase<T>::initialize()
{
}

template<typename T>
void PeerListBase<T>::handleMessage(cMessage *msg)
{
}

template<typename T>
void PeerListBase<T>::addPeer(T *peer)
{
	peers.insert(std::pair<int, T*>(peer->gateId, peer));
}

template<typename T>
void PeerListBase<T>::removePeer(int gateId)
{
	peers.erase(gateId);
}

template<typename T>
void PeerListBase<T>::removePeer(T *peer)
{
	removePeer(peer->gateId);
}

template<typename T>
T *PeerListBase<T>::getPeer(int gateId)
{
	return peers.find(gateId)->second;
}

}
