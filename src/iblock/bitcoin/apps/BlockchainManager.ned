package iblock.bitcoin.apps;

import iblock.bitcoin.apps.contract.IApp;

simple BlockchainManager like IApp
{
	parameters:
		string dispatcherModule = default("^.packetDispatcher.dispatcher");
		string peerTableModule = default("^.peerTable");
		string connectionManagerModule = default("^.connectionManager");
	gates:
		input in;
		output out;
}
