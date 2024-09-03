#include "GBM.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Define_Module(GBM);

void GBM::addBlock(Block* block)
{
	Enter_Method("addBlock()");
	blocks->add(block);
}

}
}
