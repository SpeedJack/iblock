#ifndef __IBLOCK_BITCOIN_UTILS_H_
#define __IBLOCK_BITCOIN_UTILS_H_

#include <cstdint>

namespace iblock
{
namespace bitcoin
{

std::size_t compactSize(const unsigned long value, unsigned char *result = nullptr);
unsigned long compactSizeValue(const unsigned char *result);

}
}

#endif
