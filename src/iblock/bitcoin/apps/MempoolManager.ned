package iblock.bitcoin.apps;

import iblock.bitcoin.apps.contract.IApp;

simple MempoolManager like IApp
{
	parameters:
		string dispatcherModule = default("^.packetDispatcher.dispatcher");
		string nodeManagerModule = default("nodeManager");
		string peerTableModule = default("^.peerTable");
		string connectionManagerModule = default("^.connectionManager");
	gates:
		input in;
		input dIn @directIn;
		output out;
}
