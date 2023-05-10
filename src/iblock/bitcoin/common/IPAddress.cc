#include "IPAddress.h"
#include <omnetpp.h>

namespace iblock
{
namespace bitcoin
{

int IPAddress::parseIpv6Groups(const char *& s, uint16_t *groups)
{
	int k = 0;
	while (true) {
		char *e;
		unsigned long g = strtoul(s, &e, 16);
		if (e == s) {
			if (k != 0)
				s--;
			break;
		}

		if (g > 0xFFFF)
			break;
		groups[k++] = g;
		s = e;
		if (*s != ':' || k == 8)
			break;
		s++;
	}
	return k;
}

int IPAddress::parseIpv4Groups(const char *& s, uint8_t *groups)
{
	int k = 0;
	while (true) {
		char *e;
		unsigned long g = strtoul(s, &e, 10);
		if (e == s) {
			if (k != 0)
				s--;
			break;
		}

		if (g > 0xFF)
			break;
		groups[k++] = g;
		s = e;
		if (*s != '.' || k == 4)
			break;
		s++;
	}
	return k;
}

bool IPAddress::tryParseIpv4(const char *addr)
{
	uint8_t groups[4];
	if (parseIpv4Groups(addr, groups) != 4)
		return false;
	hi = 0;
	lo = ((uint64_t)0xFFFF << 32) | ((uint64_t)groups[0] << 24) | ((uint64_t)groups[1] << 16) | ((uint64_t)groups[2] << 8) | ((uint64_t)groups[3] & 0xFF);
	return true;
}

bool IPAddress::tryParseIpv6(const char *addr)
{
	if (!addr || *addr == '\0')
		return false;
	uint16_t groups[8];
	int numGroups = parseIpv6Groups(addr, groups);

	if (*addr == ':' && *(addr + 1) == ':') {
		addr += 2;
		uint16_t groups2half[8];
		int numGroups2half = parseIpv6Groups(addr, groups2half);
		if (numGroups + numGroups2half > 8)
			return false;
		for (int i = numGroups; i < 8; ++i) {
			int j = i - 8 + numGroups2half;
			groups[i] = j < 0 ? 0 : groups2half[j];
		}
		numGroups = 8;
	}

	if (numGroups != 8)
		return false;

	hi = ((uint64_t)groups[0] << 48) | ((uint64_t)groups[1] << 32) | ((uint64_t)groups[2] << 16) | ((uint64_t)groups[3] & 0xFFFF);
	lo = ((uint64_t)groups[4] << 48) | ((uint64_t)groups[5] << 32) | ((uint64_t)groups[6] << 16) | ((uint64_t)groups[7] & 0xFFFF);

	return true;
}

bool IPAddress::tryParse(const char *addr)
{
	if (!addr || *addr == '\0')
		return false;
	if (strcmp(addr, "<unspec>") == 0) {
		hi = lo = 0;
		return true;
	}
	if (strchr(addr, '.') != nullptr) {
		if (strncasecmp(addr, "::ffff:", 7) == 0)
			addr += 7;
		return tryParseIpv4(addr);
	}

	return tryParseIpv6(addr);
}

void IPAddress::set(const char *addr)
{
	if (!tryParse(addr))
		throw ::omnetpp::cRuntimeError("IPAddress: cannot interpret address string `%s'", addr);
}

void IPAddress::findGap(uint16_t *groups, int &start, int &end)
{
	start = end = 0;
	int beg = -1;
	for (int i = 0; i < 8; ++i) {
		if (beg == -1 || groups[i] == 0) {
			beg = i;
		} else if (beg != -1 && groups[i] != 0) {
			if (i - beg >= 2 && i - beg > end - start) {
				start = beg;
				end = i;
			}
			beg = -1;
		}
	}

	if (beg != -1 && beg <= 6 && 8 - beg > end - start) {
		start = beg;
		end = 8;
	}
}

std::string IPAddress::str() const
{
	if (hi == 0 && lo == 0)
		return "<unspec>";

	uint16_t groups[8] = {
		uint16_t(hi >> 48), uint16_t(hi >> 32), uint16_t(hi >> 16), uint16_t(hi & 0xFFFF),
		uint16_t(lo >> 48), uint16_t(lo >> 32), uint16_t(lo >> 16), uint16_t(lo & 0xFFFF)
	};

	if (hi == 0 && groups[4] == 0 && groups[5] == 0xFFFF) {
		std::stringstream ss;
		ss << "::" << std::hex << groups[5] << std::dec << ":";
		ss << uint8_t(groups[6] >> 8) << "." << uint8_t(groups[6] & 0xFF) << ".";
		ss << uint8_t(groups[7] >> 8) << "." << uint8_t(groups[7] & 0xFF);
		return ss.str();
	}

	int start, end;
	findGap(groups, start, end);
	if (start == 0 && end == 8)
		return "::0";

	std::stringstream ss;
	ss << std::hex;

	for (int i = 0; i < start; ++i)
		ss << (i == 0 ? "" : ":") << groups[i];
	if (start != end)
		ss << "::";
	for (int i = end; i < 8; ++i)
		ss << (i == end ? "" : ":") << groups[i];
	return ss.str();
}

}
}
