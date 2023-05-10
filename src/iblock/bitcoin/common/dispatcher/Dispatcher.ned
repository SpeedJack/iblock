package iblock.bitcoin.common.dispatcher;

simple Dispatcher
{
	parameters:
		string peerTableModule = default("^.^.peerTable");
		bool performDiscovery = default(true);
	gates:
		input in[];
		output out[];
		inout port[];
}
