package iblock.bitcoin.apps;

import iblock.bitcoin.apps.contract.IApp;

simple TransactionGenerator like IApp
{
	parameters:
		string peerTableModule = default("^.peerTable");
		string dispatcherModule = default("^.packetDispatcher.dispatcher");
		string connectionManagerModule = default("^.connectionManager");
		string mempoolManagerModule = default("^.mempoolManager");
		string nodeManagerModule = default("nodeManager");
		string walletModule = default("^.app[0]");
		volatile double interval @unit(s) = default(exponential(10ms));
		volatile double amount @unit(BTC) = default(normal(0.001BTC, 10BTC));
		volatile int feeRate = default(geometric(1/3));
	gates:
		input in;
		output out;
}
