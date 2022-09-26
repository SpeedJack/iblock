#ifndef __IBLOCK_MESSAGEDISPATCHERBASE_H_
#define __IBLOCK_MESSAGEDISPATCHERBASE_H_

#include <omnetpp.h>
#include "../../apps/base/AppBase.h"

using namespace omnetpp;

namespace iblock
{

template<typename T>
class MessageDispatcherBase: public cSimpleModule
{
	protected:
		std::map<T, std::set<cGate*>> listeners;

		void handleIncomingMessage(cPacket *msg);
		virtual void handleMessage(cMessage *msg) override;
		virtual cMessage *packMessage(cGate *gate, cPacket *incoming) = 0;
		virtual void forwardMessage(cMessage *outgoing) = 0;

	public:
		void registerListener(AppBase *app, T msgKind);

		inline void registerListener(AppBase *app, std::vector<T> msgKinds)
		{
			for (T msgKind: msgKinds)
				registerListener(app, msgKind);
		}
};

}

#endif
