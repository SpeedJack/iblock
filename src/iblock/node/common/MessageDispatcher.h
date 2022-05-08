#ifndef __IBLOCK_NODE_COMMON_MESSAGEDISPATCHER_H_
#define __IBLOCK_NODE_COMMON_MESSAGEDISPATCHER_H_

#include <omnetpp.h>

namespace iblock
{

class MessageDispatcher: public omnetpp::cSimpleModule
{
protected:
	virtual void initialize() override;
	virtual void handleMessage(omnetpp::cMessage *msg) override;
	void handleInternalBlockMessage(omnetpp::cMessage *msg);
	void handleExternalPacket(omnetpp::cPacket *pkt);
};

}

#endif
