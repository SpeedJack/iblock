package iblock.bitcoin.apps;

import iblock.bitcoin.apps.contract.IMiner;

simple Miner like IMiner
{
	parameters:
		string dispatcherModule = default("^.packetDispatcher.dispatcher");
		string peerTableModule = default("^.peerTable");
		string connectionManagerModule = default("^.connectionManager");
		string blockchainStoreModule = default("^.blockchainManager");
		string walletAddress = default("76a914033abd090cda7389a58d936af25e7edc17da235b88ac");
		int highestTarget = default(0x1d00ffff);
		double hashRate @unit(Ghps) = default(0.01Ghps);
	gates:
		input in;
		output out;
}
