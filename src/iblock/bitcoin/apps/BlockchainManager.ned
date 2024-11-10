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
		@signal[networkDifficulty](type=double);
		@signal[mainBranchAppend](type=unsigned long);
		@signal[sideBranchAppend](type=unsigned long);
		@signal[mainBranchSwap](type=unsigned long);
		@signal[newSideBranch](type=unsigned long);
		@signal[orphanCount](type=long);
		@signal[mainBranchHeight](type=unsigned long);
		@signal[mainBranchTransactionCount](type=long);
		@signal[sideBranchLength](type=unsigned long);
		@signal[duplicateBlock](type=long);
		@statistic[networkDifficulty](source=networkDifficulty; record=vector?,stats,last; interpolationmode=sample-hold; title="Network Difficulty");
		@statistic[mainBranchLength](source=mainBranchHeight; record=vector?,stats,last; unit=blocks; interpolationmode=sample-hold; title="Main Branch Length");
		@statistic[forks](source=newSideBranch; record=vector(sum)?,count; unit=blocks; interpolationmode=sample-hold; title="Forks");
		@statistic[orphans](source=sum(orphanCount); record=vector?; unit=blocks; interpolationmode=sample-hold; title="Orphans");
		@statistic[mainBranchTransactions](source=mainBranchTransactionCount; record=vector(sum)?,histogram?,last(sum); unit=transactions; interpolationmode=sample-hold; title="Main Branch Transactions");
		@statistic[sideBranchLength](source=sideBranchLength; record=vector?,stats,histogram?,count; unit=blocks; interpolationmode=none; title="Side Branch Length");
		@statistic[duplicateBlocks](source=duplicateBlock; record=vector?,count; unit=blocks; interpolationmode=none; title="Duplicate Blocks");
		@statistic[mainBranchSwaps](source=mainBranchSwap; record=vector?,count; unit=blocks; interpolationmode=none; title="Main Branch Swaps");
	gates:
		input in;
		input dIn @directIn;
		output out;
}
