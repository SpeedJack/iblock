package iblock.simulations;
import iblock.node.bitcoin.Node;

network Test2
{
    @display("bgb=1000,1000");
    submodules:
        node0: Node;
        node1: Node;
        node2: Node;
        node3: Node;
       
    connections:
        node0.port++ <--> Link <--> node1.port++;
        node1.port++ <--> Link <--> node3.port++;
        node2.port++ <--> Link <--> node3.port++;
        node2.port++ <--> Link <--> node1.port++;
}
