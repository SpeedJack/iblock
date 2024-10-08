package iblock.bitcoin.apps;

import iblock.bitcoin.apps.BlockchainManager;

simple SelfishBCManager extends BlockchainManager
{
	parameters:
		@blockchainManager;
		int maxSelfishChainLength = default(15);
}
