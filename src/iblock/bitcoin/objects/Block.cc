#include "Block.h"
#include "BlockHeader.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Register_Class(Block)

void Block::copy(const Block& other)
{
	this->header = other.header->dup();
	unsigned long count = getTxnCount();
	setTxnArraySize(0);
	setTxnArraySize(count);
	for (unsigned int i = 0; i < count; i++)
		appendTxn(other.txn[i]->dup());
}

std::string Block::str() const
{
	std::stringstream out;
	out << "header=[" << header->str() << "], ";
	out << "txn_count=" << getTxnCount();

	std::string baseStr = Block_Base::str();
	if (!baseStr.empty())
		out << "; " << baseStr;
	return out.str();
}

}
}
