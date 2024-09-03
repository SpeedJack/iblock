#include "TxPl.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{
namespace payloads
{

Register_Class(TxPl)

unsigned char* TxPl::getRawBytes() const
{
	return nullptr;
}

void TxPl::setTransaction(::iblock::bitcoin::Transaction* tx)
{
	TxPl_Base::setTransaction(tx);
	setByteLength(tx->getByteLength());
}

}
}
}
