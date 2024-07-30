package iblock.simulations;
import iblock.bitcoin.node.MinerNode;
import iblock.bitcoin.node.UserNode;
import iblock.bitcoin.node.CustomNode;
import iblock.bitcoin.node.RelayNode;
import iblock.bitcoin.global.NodeManager;

network Test3
{
	@display("bgb=1000,1000");
	submodules:
		nodeManager: NodeManager;
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
		node20: CustomNode;
		node21: CustomNode;
		node22: CustomNode;
		node23: CustomNode;
		node24: CustomNode;
		node25: CustomNode;
		node26: CustomNode;
		node27: CustomNode;
		node28: CustomNode;
		node29: CustomNode;
		node30: RelayNode;
		node31: RelayNode;
		node32: RelayNode;
		node33: RelayNode;
		node34: RelayNode;
		node35: RelayNode;
		node36: RelayNode;
		node37: RelayNode;
		node38: RelayNode;
		node39: RelayNode;
}
