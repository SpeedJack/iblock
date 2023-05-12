#include "Payload.h"
#include <iomanip>

namespace iblock
{
namespace bitcoin
{
namespace payloads
{

Register_Abstract_Class(Payload)

size_t Payload::compactSize(unsigned long value, unsigned char *result)
{
	if (value < 253) {
		if (result != nullptr) *result = (uint8_t)value;
		return 1;
	}
	if (value <= 0xFFFF) {
		if (result != nullptr) {
			result[0] = 0xFD;
			result[1] = (uint16_t)value;
		}
		return 3;
	}
	if (value <= 0xFFFFFFFF) {
		if (result != nullptr) {
			result[0] = 0xFE;
			result[1] = (uint32_t)value;
		}
		return 5;
	}
	if (result != nullptr) {
		result[0] = 0xFF;
		result[1] = (uint64_t)value;
	}
	return 9;
}

unsigned long Payload::compactSizeValue(const unsigned char *result)
{
	if (result[0] == 0xFD)
		return (uint16_t)result[1];
	if (result[0] == 0xFE)
		return (uint32_t)result[1];
	if (result[0] == 0xFF)
		return (uint64_t)result[1];
	return (uint8_t)result[0];
}

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
