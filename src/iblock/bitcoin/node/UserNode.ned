package iblock.bitcoin.node;

import iblock.bitcoin.node.base.NodeBase;

module UserNode extends NodeBase
{
	parameters:
		numApps = default(1);
		app[0].typename = default("iblock.bitcoin.apps.TransactionGenerator");
}
