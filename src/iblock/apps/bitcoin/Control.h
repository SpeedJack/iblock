#ifndef __IBLOCK_BITCOIN_CONTROL_H_
#define __IBLOCK_BITCOIN_CONTROL_H_

#include <omnetpp.h>
#include "iblock/apps/base/AppBase.h"
#include "../../dispatcher/bitcoin/MessageDispatcher.h"
#include "../../datastructures/bitcoin/PeerList.h"
#include "../../primitives/bitcoin/Peer_m.h"
#include "../../messages/bitcoin/Packet_m.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

class Control: public AppBase
{
	protected:
		PeerList *peerListModule;
		MessageDispatcher *dispatcherModule;

		virtual void initialize() override;
		virtual void handleMessage(cMessage *msg) override;

		void broadcastVersion();

		void handleVersionMessage(const Peer *peer, const Packet *payload);
		void handleVerackMessage(const Peer *peer, const Packet *payload);
};

}
}

#endif
