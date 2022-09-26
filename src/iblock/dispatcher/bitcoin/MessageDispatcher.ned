package iblock.dispatcher.bitcoin;

import iblock.dispatcher.contract.IMessageDispatcher;

simple MessageDispatcher like IMessageDispatcher
{
	parameters:
		string peerListModule = default("^.peerlist");
	gates:
		input in[];
		output out[];
		inout port[];
}
