#ifndef __IBLOCK_BITCOIN_NODEMANAGER_H_
#define __IBLOCK_BITCOIN_NODEMANAGER_H_

#include "iblock/bitcoin/bitcoin.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API NodeManager : public ::omnetpp::cSimpleModule
{
	protected:
		std::vector<cModule *> nodes;

		virtual void initialize() override;

	public:
		NodeManager() : cSimpleModule() { nodes.clear(); }

		std::vector<cModule *> getNodes() const { Enter_Method_Silent("getNodes()"); return nodes; }
		void registerNode(cModule *node) { Enter_Method_Silent("registerNode()"); nodes.push_back(node); }
};

}
}

#endif
