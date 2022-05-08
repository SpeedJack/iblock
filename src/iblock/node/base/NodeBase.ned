package iblock.node.base;

import iblock.node.common.BlockchainStorage;
import iblock.node.common.MessageDispatcher;

module NodeBase
{
	parameters:
		@display("bgb=1256,822");

	gates:
		inout port[];

	submodules:
		dispatcher: MessageDispatcher {
			parameters:
				@display("p=125,80;is=s");
		}
		blockchainStorage: BlockchainStorage {
			parameters:
				@display("p=125,400;is=s");
		}

	connections:
		blockchainStorage.blockIn <-- dispatcher.blockOut++;
		blockchainStorage.blockOut --> dispatcher.blockIn++;

		for i=0..sizeof(port)-1 {
			port[i] <--> { @display("m=s"); } <--> dispatcher.port++;
		}
}
