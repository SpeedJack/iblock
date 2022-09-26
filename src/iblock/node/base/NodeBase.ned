package iblock.node.base;

import iblock.apps.contract.IApp;
import iblock.dispatcher.contract.IMessageDispatcher;

module NodeBase
{
	parameters:
		int numApps = default(0);
		@display("bgb=1000,1000");

	gates:
		inout port[];

	submodules:
		app[numApps]: <> like IApp {
			parameters:
				@display("p=375,76,row,150");
		}
		dispatcher: <> like IMessageDispatcher {
			parameters:
				@display("p=750,146;b=1000,5,,,,1");
		}

	connections:
		for i=0..sizeof(port)-1 {
			port[i] <--> { @display("m=s"); } <--> dispatcher.port++;
		}
		for i=0..numApps-1 {
			app[i].out --> dispatcher.in++;
			app[i].in <-- dispatcher.out++;
		}
}
