package iblock.bitcoin.apps;

import iblock.bitcoin.apps.BlockchainManager;

simple SelfishBCManager extends BlockchainManager
{
	parameters:
		@blockchainManager;
		@class(SelfishBCManager);
		int maxSelfishChainLength = default(10);
		@signal[selfishChainLength](type=unsigned long);
		@signal[selfishChainDistance](type=unsigned long);
		@signal[attackResult](type=bool);
		@statistic[selfishChainLength](source=selfishChainLength; record=vector?,max; unit=blocks; interpolationmode=sample-hold; title="Selfish Chain Length");
		@statistic[selfishChainDistance](source=selfishChainDistance; record=vector?,max; unit=blocks; interpolationmode=sample-hold; title="Selfish Chain Distance");
		@statistic[attackResult](source=attackResult; record=sum,timeavg?; unit=successes; title="Attack Result");
}
