package iblock.simulations;
import iblock.bitcoin.node.PingerNode;

channel SlowLink extends ned.DatarateChannel
{
	delay = 1s;
	datarate = 10kbps;
}

network PingPongNetwork
{
	@display("bgb=1000,1000");
	submodules:
		node0: PingerNode;
		node1: PingerNode;
		node2: PingerNode;
		node3: PingerNode;
		node4: PingerNode;
		node5: PingerNode;
	   
	connections:
		node0.port++ <--> SlowLink <--> node1.port++;
		node1.port++ <--> SlowLink <--> node3.port++;
		node2.port++ <--> SlowLink <--> node3.port++;
		node2.port++ <--> SlowLink <--> node1.port++;
		node4.port++ <--> SlowLink <--> node5.port++;
		node4.port++ <--> SlowLink <--> node2.port++;
}
