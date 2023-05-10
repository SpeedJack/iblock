#include "Payload.h"

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
		*result = (uint8_t)value;
		return 1;
	}
	if (value <= 0xFFFF) {
		result[0] = 0xFD;
		result[1] = (uint16_t)value;
		return 3;
	}
	if (value <= 0xFFFFFFFF) {
		result[0] = 0xFE;
		result[1] = (uint32_t)value;
		return 5;
	}
	result[0] = 0xFF;
	result[1] = (uint64_t)value;
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

}
}
}
