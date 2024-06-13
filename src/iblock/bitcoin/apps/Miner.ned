package iblock.bitcoin.apps;

import iblock.bitcoin.apps.contract.IMiner;

simple Miner like IMiner
{
	parameters:
		string dispatcherModule = default("^.packetDispatcher.dispatcher");
		string peerTableModule = default("^.peerTable");
		string connectionManagerModule = default("^.connectionManager");
		double hashRate @unit(Ghps) = default(1000);
	gates:
		input in;
		output out;
}
