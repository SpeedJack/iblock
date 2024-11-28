package iblock.bitcoin.apps;

import iblock.bitcoin.apps.contract.IMempoolManager;

simple MempoolManager like IMempoolManager
{
	parameters:
		string dispatcherModule = default("^.packetDispatcher.dispatcher");
		string nodeManagerModule = default("nodeManager");
		string peerTableModule = default("^.peerTable");
		string connectionManagerModule = default("^.connectionManager");
		volatile double bandwidth @unit(bps) = default(1Mbps);
		volatile double propagationDelay @unit(s) = default(exponential(5.1s));
		@signal[transactionAdded](type=unsigned long);
		@signal[transactionRemoved](type=unsigned long);
		@signal[mempoolSize](type=long);
		@statistic[transactionCount](source=count(transactionAdded)-count(transactionRemoved); record=vector?,max,last,stats?; unit=txs; interpolationmode=sample-hold; title="Transaction count");
		@statistic[mempoolSize](source=mempoolSize; record=vector?,last,stats; unit=bytes; interpolationmode=sample-hold; title="Mempool size");
	gates:
		input in;
		input dIn @directIn;
		output out;
}
