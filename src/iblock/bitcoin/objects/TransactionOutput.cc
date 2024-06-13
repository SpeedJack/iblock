#include "TransactionOutput.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Register_Class(TransactionOutput)

void TransactionOutput::copy(const TransactionOutput& other)
{
	this->value = other.value;
	this->address = other.address;
}

std::string TransactionOutput::str() const
{
	std::ostringstream out;
	out << "TODO";

	std::string baseStr = TransactionOutput_Base::str();
	if (!baseStr.empty())
		out << "; " << baseStr;
	return out.str();
}

}
}
