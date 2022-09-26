#ifndef __IBLOCK_BITCOIN_MESSAGEDISPATCHER_H_
#define __IBLOCK_BITCOIN_MESSAGEDISPATCHER_H_

#include <omnetpp.h>
#include "../base/MessageDispatcherBase.h"
#include "../../datastructures/bitcoin/PeerList.h"
#include "../../messages/bitcoin/MessageKind_m.h"
#include "../../messages/bitcoin/IncomingMessage_m.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

class MessageDispatcher: public MessageDispatcherBase<MessageKind>
{
	protected:
		PeerList *peerListModule;

		virtual void initialize() override;
		virtual IncomingMessage *packMessage(cGate *gate, cPacket *incoming) override;
		virtual void forwardMessage(cMessage *outgoing) override;
};

}
}

#endif
