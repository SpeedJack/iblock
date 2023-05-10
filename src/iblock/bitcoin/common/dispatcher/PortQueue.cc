#include "PortQueue.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Define_Module(PortQueue);

void PortQueue::initialize()
{
	timer = new cMessage("timer");
	queue = new cQueue();
	queue->setTakeOwnership(true);
}

void PortQueue::handleMessage(cMessage *msg)
{
	if (msg == timer) {
		handleTimer();
		return;
	}

	cGate *outGate = gate("out");
	cChannel *channel = outGate->getTransmissionChannel();
	if (!channel->isBusy()) {
		send(msg, outGate);
		return;
	}

	simtime_t txFinishTime = channel->getTransmissionFinishTime();
	queue->insert(msg);
	rescheduleAt(txFinishTime, timer);
}

void PortQueue::handleTimer()
{
	if (queue->isEmpty())
		return;

	cGate *outGate = gate("out");
	cChannel *channel = outGate->getTransmissionChannel();
	if (channel->isBusy()) {
		scheduleAt(channel->getTransmissionFinishTime(), timer);
		return;
	}

	cMessage *pkt = check_and_cast<cMessage *>(queue->pop());
	send(pkt, outGate);

	if(!queue->isEmpty())
		scheduleAt(pkt->getArrivalTime(), timer);
}

PortQueue::~PortQueue()
{
	cancelAndDelete(timer);
	delete queue;
}

}
}
