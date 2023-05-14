#ifndef __IBLOCK_BITCOIN_UTILS_H_
#define __IBLOCK_BITCOIN_UTILS_H_

#include <cstdint>
#include "Hash.h"

namespace iblock
{
namespace bitcoin
{

std::size_t compactSize(const unsigned long value, unsigned char *result = nullptr);
unsigned long compactSizeValue(const unsigned char *result);

inline uint32_t compactNBits(unsigned long mantissa, unsigned char exponent)
{
	return (exponent << 24) | (mantissa & 0xFFFFFF);
}

uint32_t compactNBits(const unsigned char *value, std::size_t size = 32);
unsigned char *nBitsThreshold(uint32_t nBits);
unsigned char *nBitsThreshold(unsigned long mantissa, unsigned char exponent);
bool checkNBitsThreshold(uint32_t nBits, const unsigned char *hash);
bool checkNBitsThreshold(const unsigned char *threshold, const unsigned char *hash);
bool checkNBitsThreshold(const unsigned char *mantissa, unsigned char exponent, const unsigned char *hash);

}
}

#endif
