#include "MessageDispatcher.h"

using namespace omnetpp;

namespace iblock
{

Define_Module(MessageDispatcher);

void MessageDispatcher::initialize()
{
}

void MessageDispatcher::handleMessage(cMessage *msg)
{
	if (msg->arrivedOn("blockIn"))
		handleInternalBlockMessage(msg);
	else if (msg->arrivedOn("port"))
		handleExternalPacket(check_and_cast<cPacket *>(msg));
	else
		delete msg;
}

void MessageDispatcher::handleInternalBlockMessage(cMessage *msg)
{
}

void MessageDispatcher::handleExternalPacket(cPacket *pkt)
{
}

}
