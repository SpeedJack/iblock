package iblock.simulations;
import iblock.bitcoin.node.PingerNode;
import iblock.bitcoin.node.MinerNode;
import iblock.bitcoin.global.NodeManager;

channel TestLink extends ned.DatarateChannel
{
	delay = 10ms;
	datarate = 10Mbps;
}

network Test2
{
	@display("bgb=1000,1000");
	submodules:
		node0: PingerNode;
		node1: MinerNode;
		node2: PingerNode;
		node3: PingerNode;
		nodeManager: NodeManager;
	   
	connections:
		node0.port++ <--> TestLink <--> node1.port++;
		node1.port++ <--> TestLink <--> node3.port++;
		node2.port++ <--> TestLink <--> node3.port++;
		node2.port++ <--> TestLink <--> node1.port++;
}
