package iblock.bitcoin.apps;

import iblock.bitcoin.apps.contract.IBlockchainManager;

simple BlockchainManager like IBlockchainManager
{
	parameters:
		@blockchainManager;
		string dispatcherModule = default("^.packetDispatcher.dispatcher");
		string nodeManagerModule = default("nodeManager");
		string gbmModule = default("gbm");
		string peerTableModule = default("^.peerTable");
		string connectionManagerModule = default("^.connectionManager");
		string mempoolManagerModule = default("^.mempoolManager");
	gates:
		input in;
		input dIn @directIn;
		output out;
}
