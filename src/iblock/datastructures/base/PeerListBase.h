#ifndef __IBLOCK_PEERLISTBASE_H_
#define __IBLOCK_PEERLISTBASE_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace iblock
{

template<typename T>
class PeerListBase: public cSimpleModule
{
	protected:
		std::map<int, T*> peers;

		virtual void initialize() override;
		virtual void handleMessage(cMessage *msg) override;
	public:
		void addPeer(T *peer);
		void removePeer(int gateId);
		void removePeer(T *peer);
		T *getPeer(int gateId);
};

}

#endif
