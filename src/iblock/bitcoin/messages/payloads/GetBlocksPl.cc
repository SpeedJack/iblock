#include "GetBlocksPl.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{
namespace payloads
{

Register_Class(GetBlocksPl)

unsigned char* GetBlocksPl::getRawBytes() const
{
	return nullptr;
}

}
}
}
