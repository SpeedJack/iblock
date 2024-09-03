#include "InvPl.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{
namespace payloads
{

Register_Class(InvPl)

unsigned char* InvPl::getRawBytes() const
{
	return nullptr;
}

}
}
}
