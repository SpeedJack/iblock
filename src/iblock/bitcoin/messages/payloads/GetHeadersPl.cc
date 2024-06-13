#include "GetHeadersPl.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{
namespace payloads
{

Register_Class(GetHeadersPl)

unsigned char *GetHeadersPl::getRawBytes() const
{
	return nullptr;
}

}
}
}
