#ifndef __IBLOCK_BITCOIN_PEERLIST_H_
#define __IBLOCK_BITCOIN_PEERLIST_H_

#include <omnetpp.h>
#include "../../primitives/bitcoin/Peer_m.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

class PeerList: public cSimpleModule
{
	protected:
		std::map<int, Peer *> peers;

		virtual void initialize() override;
		virtual void handleMessage(cMessage *msg) override;

	public:
		~PeerList();
		void addPeer(int gateId);
		void removePeer(int gateId);
		void removePeer(Peer *peer);
		bool hasPeer(int gateId);
		const Peer *getPeer(int gateId);
		std::vector<const Peer *> getAllPeers();
		Peer *getPeerForUpdate(int gateId)
		{
			return const_cast<Peer *>(getPeer(gateId));
		}
};

}
}

#endif
