package iblock.bitcoin.apps;

import iblock.bitcoin.apps.contract.IApp;

simple TransactionGenerator like IApp
{
	parameters:
		string peerTableModule = default("^.peerTable");
		string dispatcherModule = default("^.packetDispatcher.dispatcher");
		string connectionManagerModule = default("^.connectionManager");
		string mempoolManagerModule = default("^.mempoolManager");
		volatile double interval @unit(s) = default(exponential(10s));
		volatile double amount @unit(BTC) = default(uniform(0.0001BTC, 0.1BTC));
		volatile double fee @unit(BTC) = default(uniform(0.00001BTC, 0.00003BTC));
	gates:
		input in;
		output out;
}
