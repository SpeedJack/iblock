#ifndef __IBLOCK_BITCOIN_HASHRATE_H_
#define __IBLOCK_BITCOIN_HASHRATE_H_

#include "iblock/bitcoin/bitcoin.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API HashRate
{
	public:
		enum Unit {
			Hps = 'H',
			daHps = 'd',
			hHps = 'h',
			kHps = 'k',
			MHps = 'M',
			GHps = 'G',
			THps = 'T',
			PHps = 'P',
			EHps = 'E',
			ZHps = 'Z',
			YHps = 'Y',
		};

	protected:
		double value = 0;
		Unit unit = Hps;

		static constexpr int unitIndex(Unit u) noexcept { return u == Hps ? 0 : (u == daHps ? 1 : (u == hHps ? 2 : (u == kHps ? 3 : (u == MHps ? 4 : (u == GHps ? 5 : (u == THps ? 6 : (u == PHps ? 7 : (u == EHps ? 8 : (u == ZHps ? 9 : 10))))))))); }
		static constexpr const char* unitName(Unit u) noexcept { return u == Hps ? "H/s" : (u == daHps ? "dH/s" : (u == hHps ? "hH/s" : (u == kHps ? "kH/s" : (u == MHps ? "MH/s" : (u == GHps ? "GH/s" : (u == THps ? "TH/s" : (u == PHps ? "PH/s" : (u == EHps ? "EH/s" : (u == ZHps ? "ZH/s" : "YH/s"))))))))); }
		static constexpr int compareUnits(Unit a, Unit b) noexcept { return unitIndex(a) - unitIndex(b); }
		static constexpr bool isSameUnit(Unit a, Unit b) noexcept { return a == b; }
		static constexpr bool isLowerUnit(Unit a, Unit b) noexcept { return compareUnits(a, b) < 0; }
		static constexpr bool isHigherUnit(Unit a, Unit b) noexcept { return compareUnits(a, b) > 0; }
		static constexpr bool hasSameUnit(const HashRate& a, Unit b) noexcept { return isSameUnit(a.unit, b); }
		static constexpr bool hasLowerUnit(const HashRate& a, Unit b) noexcept { return isLowerUnit(a.unit, b); }
		static constexpr bool hasHigherUnit(const HashRate& a, Unit b) noexcept { return isHigherUnit(a.unit, b); }
		static constexpr bool hasSameUnit(const HashRate& a, const HashRate& b) noexcept { return hasSameUnit(a, b.unit); }
		static constexpr bool hasLowerUnit(const HashRate& a, const HashRate& b) noexcept { return hasLowerUnit(a, b.unit); }
		static constexpr bool hasHigherUnit(const HashRate& a, const HashRate& b) noexcept { return hasHigherUnit(a, b.unit); }

		constexpr bool hasSameUnit(Unit u) const noexcept { return hasSameUnit(*this, u); }
		constexpr bool hasLowerUnit(Unit u) const noexcept { return hasLowerUnit(*this, u); }
		constexpr bool hasHigherUnit(Unit u) const noexcept { return hasHigherUnit(*this, u); }
		constexpr bool hasSameUnit(const HashRate& other) const noexcept { return hasSameUnit(other.unit); }
		constexpr bool hasLowerUnit(const HashRate& other) const noexcept { return hasLowerUnit(other.unit); }
		constexpr bool hasHigherUnit(const HashRate& other) const noexcept { return hasHigherUnit(other.unit); }

	public:
		HashRate(double value = 0, Unit unit = Hps) : value(value), unit(unit) { }

		HashRate(const HashRate& other) { this->value = other.value; this->unit = other.unit; }

		explicit HashRate(const char* str);
		HashRate(const ::omnetpp::cValue& value, Unit unit = Hps);
		HashRate(const ::omnetpp::cPar& par, Unit defaultUnit = GHps);
		HashRate(const ::omnetpp::cProperty& prop, Unit defaultUnit = GHps);

		HashRate& operator=(const HashRate& other) { if (this == &other) return *this; this->value = other.value; this->unit = other.unit; return *this; }

		HashRate& operator=(const double& value) { return *this = HashRate(value); }

		HashRate& operator=(const char* str) { return *this = HashRate(str); }
		HashRate& operator=(const ::omnetpp::cValue& str) { return *this = HashRate(value); }
		HashRate& operator=(const ::omnetpp::cPar& par) { return *this = HashRate(par); }
		HashRate& operator=(const ::omnetpp::cProperty& prop) { return *this = HashRate(prop); }

		constexpr bool operator==(const HashRate& other) const { return hasSameUnit(other) ? this->value == other.value : (hasHigherUnit(other) ? converthr(this->value, unitName(this->unit), unitName(other.unit)) == other.value : this->value == converthr(other.value, unitName(other.unit), unitName(this->unit))); }
		constexpr bool operator!=(const HashRate& other) const { return !(*this == other); }
		constexpr bool operator<(const HashRate& other) const { return hasSameUnit(other) ? this->value < other.value : (hasHigherUnit(other) ? converthr(this->value, unitName(this->unit), unitName(other.unit)) < other.value : this->value < converthr(other.value, unitName(other.unit), unitName(this->unit))); }
		constexpr bool operator>(const HashRate& other) const { return hasSameUnit(other) ? this->value > other.value : (hasHigherUnit(other) ? converthr(this->value, unitName(this->unit), unitName(other.unit)) > other.value : this->value > converthr(other.value, unitName(other.unit), unitName(this->unit))); }
		constexpr bool operator<=(const HashRate& other) const { return hasSameUnit(other) ? this->value <= other.value : (hasHigherUnit(other) ? converthr(this->value, unitName(this->unit), unitName(other.unit)) <= other.value : this->value <= converthr(other.value, unitName(other.unit), unitName(this->unit))); }
		constexpr bool operator>=(const HashRate& other) const { return hasSameUnit(other) ? this->value >= other.value : (hasHigherUnit(other) ? converthr(this->value, unitName(this->unit), unitName(other.unit)) >= other.value : this->value >= converthr(other.value, unitName(other.unit), unitName(this->unit))); }

		const HashRate operator+(const HashRate& other) const { HashRate ret; if (hasSameUnit(other)) { ret.value = this->value + other.value; ret.unit = this->unit; } else if (hasHigherUnit(other)) { ret.value = converthr(this->value, unitName(this->unit), unitName(other.unit)) + other.value; ret.unit = other.unit; } else { ret.value = this->value + converthr(other.value, unitName(other.unit), unitName(this->unit)); ret.unit = this->unit; } return ret; }
		const HashRate operator-(const HashRate& other) const { HashRate ret; if (hasSameUnit(other)) { ret.value = this->value - other.value; ret.unit = this->unit; } else if (hasHigherUnit(other)) { ret.value = converthr(this->value, unitName(this->unit), unitName(other.unit)) - other.value; ret.unit = other.unit; } else { ret.value = this->value - converthr(other.value, unitName(other.unit), unitName(this->unit)); ret.unit = this->unit; } return ret; }
		const HashRate operator*(const HashRate& other) const { HashRate ret; if (hasSameUnit(other)) { ret.value = this->value * other.value; ret.unit = this->unit; } else if (hasHigherUnit(other)) { ret.value = converthr(this->value, unitName(this->unit), unitName(other.unit)) * other.value; ret.unit = other.unit; } else { ret.value = this->value * converthr(other.value, unitName(other.unit), unitName(this->unit)); ret.unit = this->unit; } return ret; }
		const HashRate operator/(const HashRate& other) const { HashRate ret; if (hasSameUnit(other)) { ret.value = this->value / other.value; ret.unit = this->unit; } else if (hasHigherUnit(other)) { ret.value = converthr(this->value, unitName(this->unit), unitName(other.unit)) / other.value; ret.unit = other.unit; } else { ret.value = this->value / converthr(other.value, unitName(other.unit), unitName(this->unit)); ret.unit = this->unit; } return ret; }
		HashRate& operator++() { ++this->value; return *this; }
		HashRate& operator--() { --this->value; return *this; }
		const HashRate operator++(int) { HashRate ret = *this; ++*this; return ret; }
		const HashRate operator--(int) { HashRate ret = *this; --*this; return ret; }

		HashRate& operator+=(const HashRate& other) { return *this = *this + other; }
		HashRate& operator-=(const HashRate& other) { return *this = *this - other; }
		HashRate& operator*=(const HashRate& other) { return *this = *this * other; }
		HashRate& operator/=(const HashRate& other) { return *this = *this / other; }

		bool operator==(const bool& value) const noexcept { return value == (this->value != 0.0); }
		bool operator!=(const bool& value) const noexcept { return value != (this->value != 0.0); }

		bool operator==(const double& value) const { return *this == HashRate(value); }
		bool operator!=(const double& value) const { return *this != HashRate(value); }
		bool operator<(const double& value) const { return *this < HashRate(value); }
		bool operator>(const double& value) const { return *this > HashRate(value); }
		bool operator<=(const double& value) const { return *this <= HashRate(value); }
		bool operator>=(const double& value) const { return *this >= HashRate(value); }

		const HashRate operator+(const double& value) const { return *this + HashRate(value); }
		const HashRate operator-(const double& value) const { return *this - HashRate(value); }
		const HashRate operator*(const double& value) const { return *this * HashRate(value); }
		const HashRate operator/(const double& value) const { return *this / HashRate(value); }

		HashRate& operator+=(const double& value) { return *this = *this + HashRate(value); }
		HashRate& operator-=(const double& value) { return *this = *this - HashRate(value); }
		HashRate& operator*=(const double& value) { return *this = *this * HashRate(value); }
		HashRate& operator/=(const double& value) { return *this = *this / HashRate(value); }

		friend inline const HashRate operator+(const double a, const HashRate& b) { return HashRate(a) + b; }
		friend inline const HashRate operator-(const double a, const HashRate& b) { return HashRate(a) - b; }
		friend inline const HashRate operator*(const double a, const HashRate& b) { return HashRate(a) * b; }
		friend inline const HashRate operator/(const double a, const HashRate& b) { return HashRate(a) / b; }
		friend inline bool operator==(const double a, const HashRate& b) { return HashRate(a) == b; }
		friend inline bool operator!=(const double a, const HashRate& b) { return HashRate(a) != b; }
		friend inline bool operator<(const double a, const HashRate& b) { return HashRate(a) < b; }
		friend inline bool operator>(const double a, const HashRate& b) { return HashRate(a) > b; }
		friend inline bool operator<=(const double a, const HashRate& b) { return HashRate(a) <= b; }
		friend inline bool operator>=(const double a, const HashRate& b) { return HashRate(a) >= b; }

		constexpr double Yhps() const { return converthr(value, unitName(unit), "Yhps"); }
		constexpr double Zhps() const { return converthr(value, unitName(unit), "Zhps"); }
		constexpr double Ehps() const { return converthr(value, unitName(unit), "Ehps"); }
		constexpr double Phps() const { return converthr(value, unitName(unit), "PHps"); }
		constexpr double Thps() const { return converthr(value, unitName(unit), "THps"); }
		constexpr double Ghps() const { return converthr(value, unitName(unit), "GHps"); }
		constexpr double Mhps() const { return converthr(value, unitName(unit), "MHps"); }
		constexpr double khps() const { return converthr(value, unitName(unit), "khps"); }
		constexpr double hhps() const { return converthr(value, unitName(unit), "hhps"); }
		constexpr double dahps() const { return converthr(value, unitName(unit), "dahps"); }
		constexpr double hps() const noexcept { return value; }
		double getValue(Unit unit = Hps) const { return converthr(value, "Hps", unitName(unit)); }
		constexpr double toValue() const noexcept { return value; }
		double val(Unit unit = Hps) const { return getValue(unit); }

		constexpr bool isZero() const noexcept { return value == 0.0; }

		constexpr operator bool() const noexcept { return value != 0.0; }
		constexpr operator char() const { return static_cast<char>(value); }
		constexpr operator unsigned char() const { return static_cast<unsigned char>(value); }
		constexpr operator short() const { return static_cast<short>(value); }
		constexpr operator unsigned short() const { return static_cast<unsigned short>(value); }
		constexpr operator int() const { return static_cast<int>(value); }
		constexpr operator unsigned int() const { return static_cast<unsigned int>(value); }
		constexpr operator long() const { return static_cast<long>(value); }
		constexpr operator unsigned long() const { return static_cast<unsigned long>(value); }
		constexpr operator long long() const { return static_cast<long long>(value); }
		constexpr operator unsigned long long() const { return static_cast<unsigned long long>(value); }
		constexpr operator float() const { return static_cast<float>(value); }
		constexpr operator double() const noexcept { return value; }
		constexpr operator long double() const { return static_cast<long double>(value); }

		operator char*() const { std::string s = str(); char* ret = new char[s.size() + 1]; strcpy(ret, s.c_str()); return ret; }
		operator std::string() const { return str(); }
		std::string str() const { std::stringstream ss; ss << this->value << " " << unitName(this->unit); return ss.str(); }
};

typedef HashRate hashrate_t;

}
}

#endif
