package iblock.bitcoin.node;

import iblock.bitcoin.node.base.NodeBase;

module CustomNode extends NodeBase
{
	parameters:
		numApps = default(2);
		app[0].typename = default("iblock.bitcoin.apps.Miner");
		app[1].typename = default("iblock.bitcoin.apps.TransactionGenerator");
}
