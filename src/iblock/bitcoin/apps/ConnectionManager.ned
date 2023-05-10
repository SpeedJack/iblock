package iblock.bitcoin.apps;

import iblock.bitcoin.apps.contract.IConnectionManager;

simple ConnectionManager like IConnectionManager
{
	parameters:
		string dispatcherModule = default("^.packetDispatcher.dispatcher");
		string peerTableModule = default("^.peerTable");
		string connectionManagerModule = default("^.connectionManager");
	gates:
		input in;
		output out;
}
