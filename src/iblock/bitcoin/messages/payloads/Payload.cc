#include "Payload.h"
#include <iomanip>

namespace iblock
{
namespace bitcoin
{
namespace payloads
{

Register_Abstract_Class(Payload)

std::string Payload::getRawBytesHexStr() const
{
	std::stringstream ss;
	ss << std::hex << std::setfill('0');
	unsigned char *rawBytes = getRawBytes();
	if (rawBytes == nullptr)
		return "";
	for (size_t i = 0; i < getByteLength(); i++)
		ss << std::setw(2) << static_cast<unsigned int>(rawBytes[i]);
	delete[] rawBytes;
	return ss.str();
}

}
}
}
