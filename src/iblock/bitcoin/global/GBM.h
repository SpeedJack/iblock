#ifndef __IBLOCK_BITCOIN_GBM_H_
#define __IBLOCK_BITCOIN_GBM_H_

#include "iblock/bitcoin/objects/Block.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API GBM : public ::omnetpp::cSimpleModule
{
	protected:
		::omnetpp::cArray* blocks;

		virtual void initialize() override { blocks = new ::omnetpp::cArray("blockchain"); }
		virtual void handleMessage(::omnetpp::cMessage* msg) override { delete msg; }
		virtual ~GBM() override { delete blocks; }

	public:
		GBM() : ::omnetpp::cSimpleModule() { blocks = nullptr; }
		virtual void addBlock(Block* block);
};

}
}

#endif
