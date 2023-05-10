package iblock.bitcoin.node.base;

import iblock.bitcoin.apps.contract.IApp;
import iblock.bitcoin.apps.contract.IConnectionManager;
import iblock.bitcoin.common.dispatcher.PacketDispatcher;
import iblock.bitcoin.common.PeerTable;

module NodeBase
{
	parameters:
		int numApps = default(0);
		@display("bgb=1000,1000");
	gates:
		inout port[];
	submodules:
		packetDispatcher: PacketDispatcher {
			parameters:
				@display("p=750,146;b=1000,5,,,,1");
		}
		peerTable: PeerTable {
		}
		connectionManager: <default("iblock.bitcoin.apps.ConnectionManager")> like IConnectionManager {
		}
		app[numApps]: <> like IApp {
			parameters:
				@display("p=375,76,row,150");
		}
	connections:
		for i=0..sizeof(port)-1 {
			port[i] <--> { @display("m=s"); } <--> packetDispatcher.port++;
		}
		connectionManager.out --> packetDispatcher.in++;
		connectionManager.in <-- packetDispatcher.out++;
		for i=0..numApps-1 {
			app[i].out --> packetDispatcher.in++;
			app[i].in <-- packetDispatcher.out++;
		}
}
