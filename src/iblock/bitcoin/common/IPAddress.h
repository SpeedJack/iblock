#ifndef __IBLOCK_BITCOIN_IPADDRESS_H_
#define __IBLOCK_BITCOIN_IPADDRESS_H_

#include "iblock/iblock.h"
#include <cstdint>
#include <string>

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API IPAddress
{
	private:
		uint64_t hi;
		uint64_t lo;

	protected:
		static int parseIpv6Groups(const char *& s, uint16_t *groups);
		static int parseIpv4Groups(const char *& s, uint8_t *groups);
		static void findGap(uint16_t *groups, int &start, int &end);

	public:
		IPAddress() : hi(0), lo(0) { }
		IPAddress(uint64_t hi, uint64_t lo) : hi(hi), lo(lo) { }
		IPAddress(const char *addr) { set(addr); }

		uint64_t getHi() const { return hi; }
		uint64_t getLo() const { return lo; }

		void set(const char *addr);
		bool tryParseIpv6(const char *addr);
		bool tryParseIpv4(const char *addr);
		bool tryParse(const char *addr);

		bool operator==(const IPAddress &other) const { return hi == other.hi && lo == other.lo; }
		bool operator!=(const IPAddress &other) const { return !operator==(other); }
		bool operator<(const IPAddress &other) const { return compare(other) < 0; }
		bool operator<=(const IPAddress &other) const { return compare(other) <= 0; }
		bool operator>(const IPAddress &other) const { return compare(other) > 0; }
		bool operator>=(const IPAddress &other) const { return compare(other) >= 0; }

		int compare(const IPAddress &other) const
		{
			return (hi < other.hi) ? -1 :
				(hi > other.hi) ? 1 :
				(lo < other.lo) ? -1 :
				(lo > other.lo) ? 1 : 0;
		}

		std::string str() const;
};

}
}

#endif
