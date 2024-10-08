#include "TransactionInput.h"
#include "Transaction.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Register_Class(TransactionInput)

void TransactionInput::copy(const TransactionInput& other)
{
	this->prevOutput = other.prevOutput;
	this->prevOutpoint = other.prevOutpoint;
}

void TransactionInput::updateOutpoint()
{
	if (prevOutpoint)
		delete prevOutpoint;
	prevOutpoint = new Outpoint();
	const Transaction* tx = getTransaction();
	prevOutpoint->txHash = tx->getHash();
	size_t count = tx->getTxOutCount();
	for (size_t i = 0; i < count; ++i)
		if (tx->getTxOut(i) == prevOutput.get()) {
			prevOutpoint->index = i;
			return;
		}
	prevOutpoint->index = 0xFFFFFFFF;
}

std::string TransactionInput::str() const
{
	std::ostringstream out;
	out << "TODO";

	std::string baseStr = TransactionInput_Base::str();
	if (!baseStr.empty())
		out << "; " << baseStr;
	return out.str();
}

const Transaction* TransactionInput::getTransaction() const
{
	cObject* owner = getOwner();
	return owner ? dynamic_cast<const Transaction*>(owner) : nullptr;
}

}
}
