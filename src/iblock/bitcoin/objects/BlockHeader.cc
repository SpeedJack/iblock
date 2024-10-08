#include "BlockHeader.h"
#include "Block.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Register_Class(BlockHeader);

void BlockHeader::copy(const BlockHeader& other)
{
	hash = other.hash;
	prevBlock = other.prevBlock;
}

uint32_t BlockHeader::getHeight() const
{
	if (!getPrevBlockHeader())
		return 0;
	const Block* block = getBlock();
	return block->getHeight();
}

const Block* BlockHeader::getBlock() const
{
	return check_and_cast<const Block*>(getOwner());
}

const BlockHeader* BlockHeader::getPrevBlockHeader() const
{
	if (prevBlock)
		return prevBlock->getHeader();
	return nullptr;
}

std::string BlockHeader::str() const
{
	std::stringstream out;
	out << "version=" << getVersion() << ", ";
	out << "prevBlockHeaderHash" << "TODO" << ", ";
	out << "merkleRootHash" << getMerkleRootHash() << ", ";
	out << "time=" << getTime() << ", ";
	out << "nBits=" << getNBits() << ", ";
	out << "nonce=" << getNonce();

	std::string baseStr = BlockHeader_Base::str();
	if (!baseStr.empty())
		out << "; " << baseStr;
	return out.str();
}

}
}
