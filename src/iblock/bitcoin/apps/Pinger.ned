package iblock.bitcoin.apps;

import iblock.bitcoin.apps.contract.IApp;

simple Pinger like IApp
{
	parameters:
		string dispatcherModule = default("^.packetDispatcher.dispatcher");
		string peerTableModule = default("^.peerTable");
		string connectionManagerModule = default("^.connectionManager");
		volatile double pingInterval @unit(s) = default(exponential(5s));
	gates:
		input in;
		output out;
}
