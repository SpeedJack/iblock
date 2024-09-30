#ifndef __IBLOCK_BITCOIN_UTILS_H_
#define __IBLOCK_BITCOIN_UTILS_H_

#include <cstdint>
#include <omnetpp.h>

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

#define UNIT2INT(x)		(((x[0] << 24) & 0xFF000000 ) \
				| ((x[1] << 16) & 0xFF0000) \
				| ((x[2] << 8) & 0xFF00) \
				| x[3] & 0xFF)

inline constexpr double unitMultHr(const char* unit)
{
	switch(UNIT2INT(unit)) {
	case UNIT2INT("YH/s"):
	case UNIT2INT("YH/S"):
	case UNIT2INT("Yh/s"):
	case UNIT2INT("YHps"):
	case UNIT2INT("Yhps"):
	case UNIT2INT("YHPS"):
		return 1e24;
	case UNIT2INT("ZH/s"):
	case UNIT2INT("ZH/S"):
	case UNIT2INT("Zh/s"):
	case UNIT2INT("ZHps"):
	case UNIT2INT("Zhps"):
	case UNIT2INT("ZHPS"):
		return 1e21;
	case UNIT2INT("EH/s"):
	case UNIT2INT("EH/S"):
	case UNIT2INT("Eh/s"):
	case UNIT2INT("EHps"):
	case UNIT2INT("Ehps"):
	case UNIT2INT("EHPS"):
		return 1e18;
	case UNIT2INT("PH/s"):
	case UNIT2INT("PH/S"):
	case UNIT2INT("Ph/s"):
	case UNIT2INT("PHps"):
	case UNIT2INT("Phps"):
	case UNIT2INT("PHPS"):
		return 1e15;
	case UNIT2INT("TH/s"):
	case UNIT2INT("TH/S"):
	case UNIT2INT("Th/s"):
	case UNIT2INT("THps"):
	case UNIT2INT("Thps"):
	case UNIT2INT("THPS"):
		return 1e12;
	case UNIT2INT("GH/s"):
	case UNIT2INT("GH/S"):
	case UNIT2INT("Gh/s"):
	case UNIT2INT("GHps"):
	case UNIT2INT("Ghps"):
	case UNIT2INT("GHPS"):
		return 1e9;
	case UNIT2INT("MH/s"):
	case UNIT2INT("MH/S"):
	case UNIT2INT("Mh/s"):
	case UNIT2INT("MHps"):
	case UNIT2INT("Mhps"):
	case UNIT2INT("MHPS"):
		return 1e6;
	case UNIT2INT("kH/s"):
	case UNIT2INT("kH/S"):
	case UNIT2INT("kh/s"):
	case UNIT2INT("kHps"):
	case UNIT2INT("khps"):
	case UNIT2INT("kHPS"):
		return 1e3;
	case UNIT2INT("hH/s"):
	case UNIT2INT("hH/S"):
	case UNIT2INT("hh/s"):
	case UNIT2INT("hHps"):
	case UNIT2INT("hhps"):
	case UNIT2INT("hHPS"):
		return 1e2;
	case UNIT2INT("daH/s"):
	case UNIT2INT("dah/s"):
	case UNIT2INT("daHps"):
	case UNIT2INT("dahps"):
	case UNIT2INT("daHPS"):
	case UNIT2INT("dH/s"):
	case UNIT2INT("dH/S"):
	case UNIT2INT("dh/s"):
	case UNIT2INT("dHps"):
	case UNIT2INT("dhps"):
	case UNIT2INT("dHPS"):
		return 10;
	case UNIT2INT("H/s"):
	case UNIT2INT("H/S"):
	case UNIT2INT("h/s"):
	case UNIT2INT("Hps"):
	case UNIT2INT("hps"):
	case UNIT2INT("HPS"):
	case UNIT2INT("hash"):
	case UNIT2INT("Hash"):
	case UNIT2INT("HASH"):
		return 1;
	default:
		throw ::omnetpp::cRuntimeError("Unknown unit %s", unit);
	}
}

inline constexpr double unitMult(const char* unit)
{
	switch (UNIT2INT(unit)) {
	case UNIT2INT("TAM"): /* tam-bitcoin (tonal) */
	case UNIT2INT("tam"):
		return 2814749.76710656;
	case UNIT2INT("MBTC"): /* mega-bitcoin */
	case UNIT2INT("Mbtc"):
		return 1e6;
	case UNIT2INT("kBTC"): /* kilo-bitcoin */
	case UNIT2INT("kbtc"):
	case UNIT2INT("KBTC"):
	case UNIT2INT("Kbtc"):
		return 1e3;
	case UNIT2INT("hBTC"): /* hecto-bitcoin */
	case UNIT2INT("hbtc"):
	case UNIT2INT("HBTC"):
	case UNIT2INT("Hbtc"):
		return 1e2;
	case UNIT2INT("bTBC"): /* bong-bitcoin (tonal) */
	case UNIT2INT("btbc"):
	case UNIT2INT("BTBC"):
	case UNIT2INT("Btbc"):
		return 42.94967296;
	case UNIT2INT("daBTC"): /* deca-bitcoin */
	case UNIT2INT("dabtc"):
	case UNIT2INT("DABTC"):
	case UNIT2INT("DAbtc"):
	case UNIT2INT("DBTC"):
	case UNIT2INT("Dbtc"):
		return 10;
	case UNIT2INT("mTBC"): /* mill-bitcoin (tonal) */
	case UNIT2INT("mtbc"):
		return 2.68435456;
	case UNIT2INT("BTC"): /* bitcoin */
	case UNIT2INT("btc"):
		return 1;
	case UNIT2INT("sTBC"): /* san-bitcoin (tonal) */
	case UNIT2INT("stbc"):
		return 0.16777216;
	case UNIT2INT("dBTC"): /* deci-bitcoin */
	case UNIT2INT("dbtc"):
		return 0.1;
	case UNIT2INT("tTBC"): /* ton-bitcoin (tonal) */
	case UNIT2INT("ttbc"):
		return 0.01048576;
	case UNIT2INT("cBTC"): /* centi-bitcoin */
	case UNIT2INT("cbtc"):
		return 1e-2;
	case UNIT2INT("mBTC"): /* milli-bitcoin */
	case UNIT2INT("mbtc"):
		return 1e-3;
	case UNIT2INT("TBC"): /* bitcoin (tonal) */
	case UNIT2INT("tbc"):
		return 0.00065536;
	case UNIT2INT("TBCt"): /* bitcoin-ton (tonal) */
	case UNIT2INT("tbct"):
		return 0.00004096;
	case UNIT2INT("TBCs"): /* bitcoin-san (tonal) */
	case UNIT2INT("tbcs"):
		return 0.00000256;
	case UNIT2INT("uBTC"): /* micro-bitcoin */
	case UNIT2INT("ubtc"):
		return 1e-6;
	case UNIT2INT("TBCm"): /* bitcoin-mill (tonal) */
	case UNIT2INT("tbcm"):
		return 0.00000016;
	case UNIT2INT("FIN"): /* finney */
	case UNIT2INT("fin"):
		return 1e-7;
	case UNIT2INT("TBCb"): /* bitcoin-bong (tonal) */
	case UNIT2INT("tbcb"):
	case UNIT2INT("SAT"): /* satoshi */
	case UNIT2INT("sat"):
		return 1e-8;
	case UNIT2INT("mSAT"): /* milli-satoshi */
	case UNIT2INT("msat"):
		return 1e-11;
	default:
		throw ::omnetpp::cRuntimeError("Unknown unit %s", unit);
	}
}

inline constexpr double convert(double value, const char* fromUnit, const char* toUnit)
{
	return value * unitMult(fromUnit) / unitMult(toUnit);
}

inline constexpr double converthr(double value, const char* fromUnit, const char* toUnit)
{
	return value * unitMultHr(fromUnit) / unitMultHr(toUnit);
}
}
}

#endif
