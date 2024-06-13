#include "Blockchain.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Register_Class(Blockchain);

void Blockchain::initialize()
{
	bestChainIndex = 0;
	genesisBlock = nullptr;
	forks = new cArray("forks");
	orphanForks = new cArray("orphanForks");
	forks->setTakeOwnership(false);
	orphanForks->setTakeOwnership(false);
}

void Blockchain::addBlock(Block *block)
{
	const BlockHeader *header = block->getHeader();
	const BlockHeader *parentHeader = header->getPrevBlockHeader();
	if (parentHeader == forks->get(bestChainIndex)) {
		// block extends the best chain
		forks->add(block);
	} else {
		// block extends a fork
		int i = forks->find(..);
		if (i < 0) {
			// block extends an orphan fork
			orphanForks->add(block);
		}
		else {
			// block extends a known fork
			forks->insertAt(i+1, block);
		}
	}
}

}
}
