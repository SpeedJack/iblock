package iblock.bitcoin.apps;

import iblock.bitcoin.apps.contract.IApp;

simple TransactionGenerator like IApp
{
	parameters:
		string peerTableModule = default("^.peerTable");
		string dispatcherModule = default("^.packetDispatcher.dispatcher");
		string connectionManagerModule = default("^.connectionManager");
		string mempoolManagerModule = default("^.mempoolManager");
		string walletManagerModule = default("walletManager");
		string walletModule = default("^.app[0]");
		volatile double interval @unit(s) = default(exponential(10s));
		double waitTime @unit(s) = default(60s);
		int minConfirmations = default(0);
		volatile double amount @btcUnit(BTC) = default(uniform(0.0001, 0.1));
		volatile double feeRate @btcUnit(sat) @unit(sat) = default(exponential(5sat));
		volatile int destCount = default(geometric(0.5));
		volatile double outputSplitDistrib = default(uniform(0.1, 1));
	gates:
		input in;
		output out;
}
