#ifndef __IBLOCK_BITCOIN_PINGER_H_
#define __IBLOCK_BITCOIN_PINGER_H_

#include "iblock/bitcoin/apps/base/AppBase.h"

namespace iblock
{
namespace bitcoin
{

class Pinger : public AppBase
{
	protected:
		::omnetpp::cMessage *timer;

		virtual void initialize() override;
		virtual void handlePingPacket(Peer *peer, payloads::PingPl *ping) override;
		virtual void handlePongPacket(Peer *peer, payloads::PongPl *pong) override;
		virtual void handleSelfMessage(::omnetpp::cMessage *timer) override;

	public:
		Pinger() : AppBase() { timer = nullptr; }
		virtual ~Pinger() override { cancelAndDelete(timer); }
};

}
}

#endif
