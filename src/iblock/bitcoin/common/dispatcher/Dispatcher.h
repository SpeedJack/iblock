#ifndef __IBLOCK_BITCOIN_DISPATCHER_H_
#define __IBLOCK_BITCOIN_DISPATCHER_H_

#include "iblock/bitcoin/common/PeerTable.h"
#include "iblock/bitcoin/messages/OutgoingMessage_m.h"
#include "iblock/bitcoin/messages/Packet.h"

namespace iblock
{
namespace bitcoin
{
class AppBase;

class IBLOCK_API Dispatcher : public ::omnetpp::cSimpleModule
{
	protected:
		PeerTable* peerTable;
		std::unordered_map<short, std::set<::omnetpp::cGate*>> listeners;

		virtual void initialize(int stage) override;
		virtual int numInitStages() const override { return 2; }
		virtual void handleMessage(::omnetpp::cMessage* msg) override;

		void handleIncomingPacket(Packet* pkt);
		void handleOutgoingMessage(OutgoingMessage* msg);

	public:
		Dispatcher() : ::omnetpp::cSimpleModule() { peerTable = nullptr; listeners.clear(); }
		void registerListener(AppBase* app, short kind);
};

}
}

#endif
