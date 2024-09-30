#include "HeavyObject.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Register_Abstract_Class(HeavyObject);

void HeavyObject::setBitLength(int64_t bitLength)
{
	if (bitLength < 0)
		throw cRuntimeError("setBitLength(): Negative length %" PRId64, bitLength);
	this->bitLength = bitLength;
}

void HeavyObject::addBitLength(int64_t bitLength)
{
	this->bitLength += bitLength;
	if (this->bitLength < 0)
		throw cRuntimeError("addBitLength(): Length became negative (%" PRId64 ") after adding %" PRId64, this->bitLength, bitLength);
}

}
}
