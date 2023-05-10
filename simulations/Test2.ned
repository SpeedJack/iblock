package iblock.simulations;
import iblock.bitcoin.node.PingerNode;

channel Link extends ned.DatarateChannel
{
    delay = 10ms;
    datarate = 10Mbps;
}

network Test2
{
    @display("bgb=1000,1000");
    submodules:
        node0: PingerNode;
        node1: PingerNode;
        node2: PingerNode;
        node3: PingerNode;
       
    connections:
        node0.port++ <--> Link <--> node1.port++;
        node1.port++ <--> Link <--> node3.port++;
        node2.port++ <--> Link <--> node3.port++;
        node2.port++ <--> Link <--> node1.port++;
}
