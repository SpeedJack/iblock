package iblock.bitcoin.common.dispatcher;

import iblock.bitcoin.common.dispatcher.Dispatcher;
import iblock.bitcoin.common.dispatcher.PortQueue;

module PacketDispatcher
{
	gates:
		input in[];
		output out[];
		inout port[];
	submodules:
		dispatcher: Dispatcher;
		queue[sizeof(port)]: PortQueue;
	connections:
		for i=0..sizeof(port)-1 {
			dispatcher.port$i++ <-- port$i[i];
			dispatcher.port$o[i] --> queue[i].in;
			queue[i].out --> port$o[i];
		}
		for i=0..sizeof(in)-1 {
			in[i] --> dispatcher.in++;
			out[i] <-- dispatcher.out++;
		}
}
