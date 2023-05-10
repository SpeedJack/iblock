package iblock.bitcoin.node;

import iblock.bitcoin.node.base.NodeBase;

module PingerNode extends NodeBase
{
	parameters:
		numApps = default(1);
		app[0].typename = default("iblock.bitcoin.apps.Pinger");
}
