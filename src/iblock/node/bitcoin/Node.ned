package iblock.node.bitcoin;

import iblock.node.base.NodeBase;
import iblock.datastructures.contract.IPeerList;
import iblock.apps.contract.IApp;

module Node extends NodeBase
{
	parameters:
		dispatcher.typename = default("iblock.dispatcher.bitcoin.MessageDispatcher");

	submodules:
		peerlist: <default("iblock.datastructures.bitcoin.PeerList")> like IPeerList {
			parameters:
				@display("");
		}
		control: <default("iblock.apps.bitcoin.Control")> like IApp {
			parameters:
				@display("");
		}

	connections:
		control.out --> dispatcher.in++;
		control.in <-- dispatcher.out++;
}
