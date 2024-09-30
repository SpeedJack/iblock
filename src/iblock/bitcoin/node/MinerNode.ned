package iblock.bitcoin.node;

import iblock.bitcoin.node.base.NodeBase;

module MinerNode extends NodeBase
{
	parameters:
		numApps = default(3);
		app[0].typename = default("iblock.bitcoin.apps.Wallet");
		app[1].typename = default("iblock.bitcoin.apps.Miner");
		app[2].typename = default("iblock.bitcoin.apps.TransactionGenerator");
}
