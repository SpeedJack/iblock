package iblock.bitcoin.apps;

import iblock.bitcoin.apps.contract.IMiner;

simple Miner like IMiner
{
	parameters:
		string dispatcherModule = default("^.packetDispatcher.dispatcher");
		string peerTableModule = default("^.peerTable");
		string connectionManagerModule = default("^.connectionManager");
		string blockchainStoreModule = default("^.blockchainManager");
		string mempoolManagerModule = default("^.mempoolManager");
		string walletModule = default("^.app[0]");
		int highestTarget = default(0x1d00ffff);
		double hashRate @unit(Ghps) = default(0.0003579139413333334Ghps);
	gates:
		input in;
		output out;
}
