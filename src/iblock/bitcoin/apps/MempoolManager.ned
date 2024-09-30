package iblock.bitcoin.apps;

import iblock.bitcoin.apps.contract.IMempoolManager;

simple MempoolManager like IMempoolManager
{
	parameters:
		string dispatcherModule = default("^.packetDispatcher.dispatcher");
		string nodeManagerModule = default("nodeManager");
		string gmmModule = default("gmm");
		string peerTableModule = default("^.peerTable");
		string connectionManagerModule = default("^.connectionManager");
	gates:
		input in;
		input dIn @directIn;
		output out;
}
