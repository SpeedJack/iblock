#include "TransactionOutput.h"
#include "Transaction.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Register_Class(TransactionOutput)

std::string TransactionOutput::str() const
{
	std::ostringstream out;
	out << "TODO";

	std::string baseStr = TransactionOutput_Base::str();
	if (!baseStr.empty())
		out << "; " << baseStr;
	return out.str();
}

const Transaction* TransactionOutput::getTransaction() const
{
	const cObject* owner = getOwner();
	return owner ? dynamic_cast<const Transaction*>(owner) : nullptr;
}

bool TransactionOutput::isCoinbase() const
{
	const Transaction* tx = getTransaction();
	return tx ? tx->isCoinbase() : false;
}

}
}
