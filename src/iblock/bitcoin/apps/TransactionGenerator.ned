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
		volatile double txGenInterval @unit(s) = default(exponential(10s));
		int minConfirmations = default(0);
		volatile double amount @btcUnit(BTC) = default(uniform(0.0001, 0.1));
		volatile double feeRate @btcUnit(sat) @unit(sat) = default(exponential(5sat));
		volatile int destCount = default(geometric(0.5));
		volatile double outputSplitDistrib = default(uniform(0.1, 1));
		@signal[txInputValue](type=unsigned long);
		@signal[txOutputValue](type=unsigned long);
		@signal[txFee](type=unsigned long);
		@signal[txSize](type=unsigned long);
		@signal[txInputCount](type=unsigned long);
		@signal[txOutputCount](type=unsigned long);
		@statistic[createdTransactions](source=count(txSize); record=vector?,last,stats?; unit=txs; interpolationmode=sample-hold; title="Created Transactions");
		@statistic[transactionInputCount](source=txInputCount; record=vector?,histogram; units=inputs; interpolationmode=none; title="Transaction Input Count");
		@statistic[transactionOutputCount](source=txOutputCount; record=vector?,histogram; units=outputs; interpolationmode=none; title="Transaction Output Count");
		@statistic[transactionInputValue](source=txInputValue; record=vector?,sum?,histogram; unit=sat; interpolationmode=none; title="Transaction Input Value");
		@statistic[transactionOutputValue](source=txOutputValue; record=vector?,sum,histogram; unit=sat; interpolationmode=none; title="Transaction Output Value");
		@statistic[transactionFee](source=txFee; record=vector?,sum,histogram; unit=sat; interpolationmode=none; title="Transaction Fee");
		@statistic[transactionSize](source=txSize; record=vector?,sum?,histogram; unit=bytes; interpolationmode=none; title="Transaction Size");
		@statistic[transactionFeeRate](source=txFee/txSize; record=vector?,histogram; unit=sat/byte; interpolationmode=none; title="Transaction Fee Rate");
	gates:
		input in;
		output out;
}
