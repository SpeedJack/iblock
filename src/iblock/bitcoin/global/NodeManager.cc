#include "NodeManager.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Define_Module(NodeManager);

void NodeManager::initialize(int stage)
{
	if (stage < 2)
		return;
	cTopology topo;
	topo.extractByProperty("bitcoinNode");
	for (int i = 0; i < topo.getNumNodes(); ++i) {
		cModule* node = topo.getNode(i)->getModule();
		networkNodes.push_back(node);
	}
}

cppcoro::generator<cModule*> NodeManager::nodes() const
{
	for (cModule* node : networkNodes)
		co_yield node;
}

}
}
