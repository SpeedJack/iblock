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
		@signal[minedBlock](type=unsigned long);
		@signal[processedTransactions](type=unsigned long);
		@signal[blockSize](type=unsigned long);
		@signal[blockReward](type=unsigned long);
		@signal[blockTime](type=simtime_t);
		@statistic[blocksMined](source=minedBlock; record=vector?,count,timeavg; unit=blocks; interpolationmode=sample-hold; title="Blocks Mined");
		@statistic[transactionsProcessed](source=processedTransactions; record=vector?,stats,sum,histogram?; unit=transactions; interpolationmode=sample-hold; title="Transactions Processed");
		@statistic[blockSize](source=blockSize; record=vector?,stats,sum?,histogram?; unit=bytes; interpolationmode=sample-hold; title="Block Sizes");
		@statistic[blockReward](source=blockReward; record=vector?,stats,sum,histogram?; unit=sat; interpolationmode=sample-hold; title="Block Rewards");
		@statistic[blockTime](source=blockTime; record=vector?,stats,histogram?; unit=s; interpolationmode=sample-hold; title="Block Time");
		@statistic[blockRewardPerByte](source=blockReward/blockSize; record=vector?,stats,histogram?; unit=sat/byte; interpolationmode=none; title="Block Reward per Byte");
	gates:
		input in;
		output out;
}
