#ifndef __IBLOCK_MESSAGEDISPATCHERBASEIMPL_H_
#define __IBLOCK_MESSAGEDISPATCHERBASEIMPL_H_

#include "MessageDispatcherBase.h"

namespace iblock
{

template<typename T>
void MessageDispatcherBase<T>::handleIncomingMessage(cPacket *msg)
{
	T kind = static_cast<T>(msg->getKind());
	typename std::map<T, std::set<cGate *>>::iterator it = listeners.find(kind);

	if (it == listeners.end())
		EV_WARN << "MessageDispatcher: No listener found for message kind " << kind << std::endl;

	std::set<cGate*> gateSet = it->second;
	unsigned int remaining = gateSet.size();
	for (std::set<cGate*>::iterator it = gateSet.begin(); it != gateSet.end(); ++it) {
		cGate *gate = *it;
		cMessage *incoming = packMessage(msg->getArrivalGate(), remaining == 1 ? msg : msg->dup());
		send(incoming, gate);
		remaining--;
	}
}

template<typename T>
void MessageDispatcherBase<T>::handleMessage(cMessage *msg)
{
	if (msg->getArrivalGate()->getBaseId() == gateBaseId("port$i")) {
		this->handleIncomingMessage(check_and_cast<cPacket *>(msg));
	} else {
		this->forwardMessage(msg);
		delete msg;
	}
}

#define ENUM_NAME(type) 		opp_typename(typeid(type))
#define ENUM_ITEM_NAME(type, value)	cEnum::get(ENUM_NAME(type))->getStringFor(value)

template<typename T>
void MessageDispatcherBase<T>::registerListener(AppBase *app, T msgKind)
{
	Enter_Method_Silent("registerListener(%s, %s)", app->getFullPath().c_str(), ENUM_ITEM_NAME(T, msgKind));

	cGate *gate = app->gate("in")->getPathStartGate();
	if (!gate || gate->getOwnerModule() != this)
		error("App %s called registerListener but it is not connected to this module", app->getFullPath().c_str());

	if (listeners.find(msgKind) == listeners.end()) {
		std::set<cGate*> gateSet;
		gateSet.insert(gate);
		listeners.insert(std::pair<T, std::set<cGate*>>(msgKind, gateSet));
	} else {
		std::set<cGate*> gateSet = listeners.find(msgKind)->second;
		gateSet.insert(gate);
	}
	EV_INFO << "Registered " << app->getFullPath() << " for message kind " << ENUM_NAME(T) << "." << ENUM_ITEM_NAME(T, msgKind) << std::endl;
}

}

#endif
