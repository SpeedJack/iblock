#ifndef __IBLOCK_BITCOIN_APPBASE_H_
#define __IBLOCK_BITCOIN_APPBASE_H_

#include <omnetpp.h>
#include "iblock/bitcoin/apps/contract/IConnectionManager.h"
#include "iblock/bitcoin/common/dispatcher/Dispatcher.h"
#include "iblock/bitcoin/messages/IncomingMessage_m.h"
#include "iblock/bitcoin/messages/payloads/payloads.h"

namespace iblock
{
namespace bitcoin
{

class AppBase : public ::omnetpp::cSimpleModule
{
	protected:
		PeerTable *peerTable;
		Dispatcher *dispatcher;
		IConnectionManager *connectionManager;
		std::unordered_map<::omnetpp::msgid_t, OutgoingMessage *> queue;

		AppBase() : ::omnetpp::cSimpleModule() { peerTable = nullptr; dispatcher = nullptr; connectionManager = nullptr; }
		virtual void initialize() override;
		virtual void handleMessage(::omnetpp::cMessage *msg) override;
		virtual void handleSelfMessage(::omnetpp::cMessage *msg);
		virtual void handleIncomingMessage(IncomingMessage *msg);
		virtual void handleOtherMessage(::omnetpp::cMessage *msg);

		Peer *getPeer(IncomingMessage *msg) { return const_cast<Peer *>(::omnetpp::check_and_cast<const Peer *>(msg->getPeer())); }

		virtual void onPeerConnected(bool success, ::omnetpp::msgid_t msgid);

		virtual void sendPacket(OutgoingMessage *msg, bool connnect = true);

		virtual void sendPacket(Packet *packet, std::initializer_list<Peer *> peers, bool connect = true, bool waitAll = false);
		virtual void sendPacket(Packet *packet, Peer *peer, bool connect = true)
		{ sendPacket(new OutgoingMessage(peer, packet), connect); }
		virtual void broadcastPacket(Packet *packet, bool connect = false, bool waitAll = false)
		{ sendPacket(packet, {}, connect, waitAll); }

		virtual void sendPacket(payloads::Payload *payload, std::initializer_list<Peer *> peers, bool connect = true, bool waitAll = false)
		{ sendPacket(new Packet(payload), peers, connect, waitAll); }
		virtual void sendPacket(payloads::Payload *payload, Peer *peer, bool connect = true)
		{ sendPacket(new Packet(payload), peer, connect); }
		virtual void broadcastPacket(payloads::Payload *payload, bool connect = false, bool waitAll = false)
		{ broadcastPacket(new Packet(payload), connect, waitAll); }

		virtual void handlePingPacket(Peer *peer, Packet *pkt) { handlePingPacket(peer, ::omnetpp::check_and_cast<payloads::PingPl *>(pkt->decapsulate())); }
		virtual void handlePingPacket(Peer *peer, payloads::PingPl *ping) { delete ping; } // TODO: warn

		virtual void handlePongPacket(Peer *peer, Packet *pkt) { handlePongPacket(peer, ::omnetpp::check_and_cast<payloads::PongPl *>(pkt->decapsulate())); }
		virtual void handlePongPacket(Peer *peer, payloads::PongPl *pong) { delete pong; }

		virtual void handleVersionPacket(Peer *peer, Packet *pkt) { handleVersionPacket(peer, ::omnetpp::check_and_cast<payloads::VersionPl *>(pkt->decapsulate())); }
		virtual void handleVersionPacket(Peer *peer, payloads::VersionPl *version) { delete version; }

		virtual void handleVerackPacket(Peer *peer, Packet *pkt) { handleVerackPacket(peer, ::omnetpp::check_and_cast<payloads::VerackPl *>(pkt->decapsulate())); }
		virtual void handleVerackPacket(Peer *peer, payloads::VerackPl *verack) { delete verack; }

		virtual void handleUnknownPacket(Peer *peer, Packet *pkt) { delete pkt; }

		virtual ~AppBase() override;
};

}
}

#endif
