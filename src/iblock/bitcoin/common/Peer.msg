import iblock.bitcoin.common.Network;
import iblock.bitcoin.common.IPAddress;

cplusplus {{
	#include <functional>
}}

namespace iblock::bitcoin;

enum ConnectionState
{
	UNCONNECTED = 0x00;
	HALF_CONNECTED = 0x01;
	CONNECTED = 0x03;
}

class Peer extends cObject
{
	unsigned int id;
	::omnetpp::cGate *gate = nullptr;
	Network network = Network::Mainnet;
	ConnectionState state = ConnectionState::UNCONNECTED;
	int32_t version = -1;
	uint64_t services = 0;
	IPAddress address = "::ffff:127.0.0.1" @byValue;
	uint16_t port = 8333;
	string userAgent = "";
	int32_t knownHeight = 0;
	bool relay = true;
}

cplusplus(Peer) {{
	private:
		std::function<void(::omnetpp::cGate *, IPAddress, uint16_t)> updateCb;
	public:
		Peer(::omnetpp::cGate *gate, Network network = Network::Mainnet, IPAddress address = "::ffff:127.0.0.1", uint16_t port = 8333) : gate(gate), network(network), address(address), port(port) { }
		Peer(::omnetpp::cGate *gate, IPAddress address, uint16_t port = 8333) : Peer(gate, Network::Mainnet, address, port) { }

		virtual bool isUnconnected() const { return state == ConnectionState::UNCONNECTED; }
		virtual bool isHalfConnected() const { return state == ConnectionState::HALF_CONNECTED; }
		virtual bool isConnected() const { return state == ConnectionState::CONNECTED; }
		virtual bool isVersionSent() const { return isConnected() || isHalfConnected(); }
		virtual void setConnected() { setState(ConnectionState::CONNECTED); }
		virtual void setUnconnected() { setState(ConnectionState::UNCONNECTED); }
		virtual void setHalfConnected() { setState(ConnectionState::HALF_CONNECTED); }
		virtual void setVersionSent() { if (isUnconnected()) setHalfConnected(); }

		virtual int getGateId() const { return gate->getId(); }
		virtual void setUpdateCallback(std::function<void(::omnetpp::cGate *, IPAddress, uint16_t)> cb) { updateCb = cb; }
}}

cplusplus(Peer::setGate) {{
	if (this->gate != gate) updateCb(gate, address, port);
}}

cplusplus(Peer::setPort) {{
	if (this->port != port) updateCb(gate, address, port);
}}

cplusplus(Peer::setAddress) {{
	if (this->address != address) updateCb(gate, address, port);
}}
