package iblock.bitcoin.apps;

import iblock.bitcoin.apps.contract.IMiner;

simple Miner like IMiner
{
	parameters:
		@miner;
		string dispatcherModule = default("^.packetDispatcher.dispatcher");
		string peerTableModule = default("^.peerTable");
		string connectionManagerModule = default("^.connectionManager");
		string blockchainStoreModule = default("^.blockchainManager");
		string mempoolManagerModule = default("^.mempoolManager");
		string walletModule = default("^.app[0]");
		int highestTarget = default(0x1d00ffff);
		double hashRate @hrUnit(Thps) = default(20);
		@signal[blockMined](type=long);
		@signal[processedTransactions](type=long);
	gates:
		input in;
		output out;
}
