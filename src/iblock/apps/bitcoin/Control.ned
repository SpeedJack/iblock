package iblock.apps.bitcoin;

import iblock.apps.contract.IApp;

simple Control like IApp
{
	parameters:
		string peerListModule = default("^.peerlist");
		string dispatcherModule = default("^.dispatcher");
		double initialDelay @unit(s) = default(intuniform(0s, 10s));
	gates:
		input in;
		output out;
}
