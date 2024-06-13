#include "PongPl.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{
namespace payloads
{

Register_Class(PongPl)

unsigned char *PongPl::getRawBytes() const
{
	unsigned char *rawBytes = new unsigned char[8];
	std::memcpy(rawBytes, &nonce, 8);
	return rawBytes;
}

}
}
}
