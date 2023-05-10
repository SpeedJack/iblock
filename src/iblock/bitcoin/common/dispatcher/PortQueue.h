#ifndef __IBLOCK_BITCOIN_PORTQUEUE_H_
#define __IBLOCK_BITCOIN_PORTQUEUE_H_

#include <omnetpp.h>

namespace iblock
{
namespace bitcoin
{

class PortQueue : public ::omnetpp::cSimpleModule
{
	protected:
		::omnetpp::cMessage *timer;
		::omnetpp::cQueue *queue;

		virtual void initialize() override;
		virtual void handleMessage(::omnetpp::cMessage *msg) override;
		void handleTimer();

	public:
		PortQueue() : ::omnetpp::cSimpleModule() { timer = nullptr; queue = nullptr; }
		virtual ~PortQueue() override;
};

}
}

#endif
