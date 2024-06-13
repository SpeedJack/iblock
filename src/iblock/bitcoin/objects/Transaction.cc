#include "Transaction.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Register_Class(Transaction);

void Transaction::copy(const Transaction& other)
{
	this->version = other.version;
	unsigned long count = getTxInCount();
	setTxInArraySize(0);
	setTxInArraySize(count);
	for (unsigned long i = 0; i < count; i++)
		appendTxIn(other.txIn[i]->dup());
	count = getTxOutCount();
	setTxOutArraySize(0);
	setTxOutArraySize(count);
	for (unsigned long i = 0; i < count; i++)
		appendTxOut(other.txOut[i]->dup());
}

std::string Transaction::str() const
{
	std::ostringstream out;
	out << "TODO";

	std::string baseStr = Transaction_Base::str();
	if (!baseStr.empty())
		out << "; " << baseStr;
	return out.str();
}

}
}
