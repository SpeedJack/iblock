#include "PingPl.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{
namespace payloads
{

Register_Class(PingPl);

unsigned char *PingPl::getRawBytes() const
{
	unsigned char *rawBytes = new unsigned char[8];
	std::memcpy(rawBytes, &nonce, 8);
	return rawBytes;
}

}
}
}
