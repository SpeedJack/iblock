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
	this->prevOutput = other.prevOutput; // FIXME: ownership? delete?
	//this->signatureScript = other.signatureScript; // FIXME: delete?
	this->sequence = other.sequence;
}

void TransactionInput::updateOutpoint()
{
	prevOutpoint = new Outpoint();
	Transaction *tx = check_and_cast<Transaction *>(getOwner());
	prevOutpoint->txHash = tx->getHash();
	size_t count = tx->getTxOutCount();
	for (size_t i = 0; i < count; ++i)
		if (tx->getTxOut(i) == prevOutput) {
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

}
}
