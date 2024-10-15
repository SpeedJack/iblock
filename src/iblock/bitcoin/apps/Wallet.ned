package iblock.bitcoin.apps;

import iblock.bitcoin.apps.contract.IApp;

simple Wallet like IApp
{
	parameters:
		@wallet;
		string dispatcherModule = default("^.packetDispatcher.dispatcher");
		string peerTableModule = default("^.peerTable");
		string connectionManagerModule = default("^.connectionManager");
		string blockchainManagerModule = default("^.blockchainManager");
		string mempoolManagerModule = default("^.mempoolManager");
		string nodeManagerModule = default("nodeManager");
		double startingBalance @btcUnit("BTC") = default(50);
		int coinbaseMaturity = default(100);
		@signal[addUTXO](type=unsigned long);
		@signal[removeUTXO](type=unsigned long);
		@signal[createAddress](type=unsigned long);
		@signal[utxoBTL](type=unsigned long);
		@signal[miningEarn](type=unsigned long);
		@statistic[utxoCount](source=count(addUTXO)-count(removeUTXO); record=vector?,max,last,stats?; unit=sat; interpolationmode=sample-hold; title="UTXO count");
		@statistic[walletBalance](source=sum(addUTXO)-sum(removeUTXO); record=vector?,max,last,stats?; unit=sat; interpolationmode=sample-hold; title="Wallet balance");
		@statistic[createdAddresses](source=count(createAddress); record=vector?,last; unit=addresses; interpolationmode=sample-hold; title="Created addresses");
		@statistic[utxoDuration](source=utxoBTL; record=vector?,stats,histogram?; unit=blocks; interpolationmode=sample-hold; title="UTXO duration");
		@statistic[miningEarnings](source=sum(miningEarn); record=vector?,last,histogram?; unit=sat; interpolationmode=sample-hold; title="Mining earnings");
	gates:
		input in;
		output out;
}
