package iblock.simulations;
import iblock.bitcoin.node.MinerNode;
import iblock.bitcoin.node.UserNode;
import iblock.bitcoin.node.CustomNode;
import iblock.bitcoin.node.RelayNode;
import iblock.bitcoin.global.NodeManager;
import iblock.bitcoin.global.WalletManager;
import iblock.bitcoin.global.GBM;
import iblock.bitcoin.global.GMM;

network SelfishMining
{
	parameters:
		@display("bgb=1000,1000");
		@statistic[minedBlocks](title="Mined Blocks"; source=blockMined; unit=blocks; record=sum);
		@statistic[processedTxs](title="Processed Transactions"; source=processedTransactions; unit=txs; record=sum);
		node0.blockchainManager.typename = "iblock.bitcoin.apps.SelfishBCManager";
		node0.app[1].hashRate = 40;
	submodules:
		nodeManager: NodeManager;
		walletManager: WalletManager;
		gbm: GBM;
		gmm: GMM;
		node0: MinerNode;
		node1: MinerNode;
		node2: MinerNode;
		node3: MinerNode;
		node4: MinerNode;
		node10: UserNode;
		node11: UserNode;
		node12: UserNode;
		node13: UserNode;
		node14: UserNode;
		node15: UserNode;
		node16: UserNode;
		node17: UserNode;
		node18: UserNode;
		node19: UserNode;
}
