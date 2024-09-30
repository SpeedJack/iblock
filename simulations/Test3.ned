package iblock.simulations;
import iblock.bitcoin.node.MinerNode;
import iblock.bitcoin.node.UserNode;
import iblock.bitcoin.node.CustomNode;
import iblock.bitcoin.node.RelayNode;
import iblock.bitcoin.global.NodeManager;
import iblock.bitcoin.global.WalletManager;
import iblock.bitcoin.global.GBM;
import iblock.bitcoin.global.GMM;

network Test3
{
	@display("bgb=1000,1000");
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
		node5: MinerNode;
		node6: MinerNode;
		node7: MinerNode;
		node8: MinerNode;
		node9: MinerNode;
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
		node20: UserNode;
		node21: UserNode;
		node22: UserNode;
		node23: UserNode;
		node24: UserNode;
		node25: UserNode;
		node26: UserNode;
		node27: UserNode;
		node28: UserNode;
		node29: UserNode;
}
