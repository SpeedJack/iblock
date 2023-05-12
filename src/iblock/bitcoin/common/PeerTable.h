#ifndef __IBLOCK_BITCOIN_PEERTABLE_H_
#define __IBLOCK_BITCOIN_PEERTABLE_H_

#include "iblock/IblockCommon.h"
#include <unordered_map>
#include "Peer_m.h"
#include "IPAddress.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API PeerTable : public ::omnetpp::cSimpleModule
{
	protected:
		struct PeerIP
		{
			IPAddress address;
			uint16_t port;

			bool operator==(const PeerIP& other) const
			{
				return address == other.address && port == other.port;
			}

			bool operator<(const PeerIP& other) const
			{
				return address < other.address || (address == other.address && port < other.port);
			}
		};
		struct PeerIPHash
		{
			std::size_t operator()(const PeerIP& ip) const
			{
				return std::hash<uint64_t>()(ip.address.getHi()) ^ std::hash<uint64_t>()(ip.address.getLo()) ^ std::hash<uint16_t>()(ip.port);
			}
		};
		struct PeerId
		{
			int gateId;
			PeerIP ip;

			bool operator==(const PeerId& other) const
			{
				return gateId == other.gateId && ip == other.ip;
			}

			bool operator<(const PeerId& other) const
			{
				return gateId < other.gateId || (gateId == other.gateId && ip < other.ip);
			}
		};
		// struct PeerIdHash
		// {
		// 	std::size_t operator()(const PeerId& id) const
		// 	{
		// 		return std::hash<int>()(id.gateId) ^ PeerIPHash()(id.ip);
		// 	}
		// };
		// struct PeerIdEqual
		// {
		// 	bool operator()(const PeerId& lhs, const PeerId& rhs) const
		// 	{
		// 		return lhs.gateId == rhs.gateId && lhs.ip.address == rhs.ip.address && lhs.ip.port == rhs.ip.port;
		// 	}
		// };

		// struct PeerIdCompare
		// {
		// 	bool operator()(const PeerId& lhs, const PeerId& rhs) const
		// 	{
		// 		return lhs.gateId < rhs.gateId || 
		// 			(lhs.gateId == rhs.gateId && lhs.ip.address < rhs.ip.address) ||
		// 			(lhs.gateId == rhs.gateId && lhs.ip.address == rhs.ip.address && lhs.ip.port < rhs.ip.port);
		// 	}
		// };

		std::map<PeerId, Peer *> peers;
		std::unordered_multimap<int, Peer *> gateMap;
		std::unordered_multimap<PeerIP, Peer *, PeerIPHash> IPMap;
		unsigned int nextId;

		void removeGateMap(const Peer *peer);
		void removeIPMap(const Peer *peer);

		void updatePeer(Peer *peer, int gateId, IPAddress address, uint16_t port);
		void updatePeer(Peer *peer, ::omnetpp::cGate *gate, IPAddress address, uint16_t port) { updatePeer(peer, gate->getId(), address, port); }

		virtual void handleMessage(::omnetpp::cMessage *msg) override { delete msg; }

	public:
		virtual void initialize() override { nextId = 0; peers.clear(); gateMap.clear(); IPMap.clear(); }
		virtual ~PeerTable() override;

		void addPeer(Peer *peer);

		std::vector<Peer *> getAllPeers(bool connected = false) const;
		int getPeerCount() const { Enter_Method("getPeerCount"); return peers.size(); }

		std::vector<Peer *> getPeers(Network network = Network::Mainnet, bool connected = false) const;
		std::vector<Peer *> getPeers(bool connected, Network network = Network::Mainnet) const { return getPeers(network, connected); }

		std::vector<Peer *> getPeers(int gateId, Network network = Network::Mainnet, bool connected = false) const;
		std::vector<Peer *> getPeers(int gateId, bool connected, Network network = Network::Mainnet) const { return getPeers(gateId, network, connected); }
		std::vector<Peer *> getPeers(::omnetpp::cGate *gate, Network network = Network::Mainnet, bool connected = false) const { return getPeers(gate->getId(), network, connected); }
		std::vector<Peer *> getPeers(::omnetpp::cGate *gate, bool connected, Network network = Network::Mainnet) const { return getPeers(gate->getId(), network, connected); }

		std::vector<Peer *> getPeers(IPAddress address, uint16_t port = 8333, Network network = Network::Mainnet, bool connected = false) const;
		std::vector<Peer *> getPeers(IPAddress address, uint16_t port, bool connected, Network network = Network::Mainnet) const { return getPeers(address, port, network, connected); }
		std::vector<Peer *> getPeers(IPAddress address, bool connected, uint16_t port = 8333, Network network = Network::Mainnet) const { return getPeers(address, port, network, connected); }

		Peer *getPeer(int gateId, IPAddress address, uint16_t port = 8333) const;
		Peer *getPeer(int gateId) const;
		Peer *getPeer(::omnetpp::cGate *gate, IPAddress address, uint16_t port = 8333) const { return getPeer(gate->getId(), address, port); }
		Peer *getPeer(::omnetpp::cGate *gate) const { return getPeer(gate->getId()); }
		Peer *getPeer(IPAddress address, uint16_t port = 8333) const;

		Peer *getPeerAtIndex(unsigned int index) const;

		Peer *removePeer(Peer *peer);
		Peer *removePeer(int gateId, IPAddress address, uint16_t port = 8333) { return removePeer(getPeer(gateId, address, port)); }
		Peer *removePeer(::omnetpp::cGate *gate, IPAddress address = "::ffff:127.0.0.1", uint16_t port = 8333) { return removePeer(getPeer(gate, address, port)); }

		bool hasPeer(int gateId) const;
		bool hasPeer(IPAddress address, uint16_t port = 8333) const;
		bool hasPeer(int gateId, IPAddress address, uint16_t port = 8333) const;
		bool hasPeer(::omnetpp::cGate *gate) const { return hasPeer(gate->getId()); }
		bool hasPeer(::omnetpp::cGate *gate, IPAddress address, uint16_t port = 8333) const { return hasPeer(gate->getId(), address, port); }

		friend class Peer;
};

}
}

#endif
