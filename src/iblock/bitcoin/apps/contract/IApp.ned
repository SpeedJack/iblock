package iblock.bitcoin.apps.contract;

moduleinterface IApp
{
	parameters:
		string peerTableModule;
		string dispatcherModule;
		string connectionManagerModule;
	gates:
		input in;
		output out;
}
