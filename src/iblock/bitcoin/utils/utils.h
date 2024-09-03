#ifndef __IBLOCK_BITCOIN_UTILS_H_
#define __IBLOCK_BITCOIN_UTILS_H_

#include <cstdint>

namespace iblock
{
namespace bitcoin
{

#define COMPACT_SIZE(x)		(((x) < 253) ? 1 : (((x) <= 0xFFFF) ? 3 : (((x) <= 0xFFFFFFFF) ? 5 : 9)))
#define SATOSHIS(x)		((x) < 0 ? 0 : static_cast<int64_t>((x) * 1e8))
#define BTCS(x)			((x) < 0 ? 0.0 : (static_cast<double>(x) / 1e8))

std::size_t compactSize(const unsigned long value, unsigned char* result);
inline std::size_t compactSize(const unsigned long value) { return COMPACT_SIZE(value); }
unsigned long compactSizeValue(const unsigned char* result);
inline int64_t satoshis(const double btcs) { return SATOSHIS(btcs); }
inline double btcs(const int64_t satoshis) { return BTCS(satoshis); }

}
}

#endif
