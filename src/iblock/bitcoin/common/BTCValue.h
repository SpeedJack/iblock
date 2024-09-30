#ifndef __IBLOCK_BITCOIN_BTCVALUE_H_
#define __IBLOCK_BITCOIN_BTCVALUE_H_

#include "iblock/bitcoin/bitcoin.h"
#include <sstream>

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API BTCValue
{
	protected:
		unsigned long long value; // max ~90mln (9e7) BTC @ full precision

	public:
		BTCValue() { this->value = 0; }
		explicit BTCValue(bool value) : BTCValue(value ? 1ULL : 0ULL) {}
		BTCValue(char value) : BTCValue(static_cast<unsigned long long>(value)) {}
		BTCValue(unsigned char value) : BTCValue(static_cast<unsigned long long>(value)) {}
		BTCValue(short value) : BTCValue(static_cast<unsigned long long>(value)) {}
		BTCValue(unsigned short value) : BTCValue(static_cast<unsigned long long>(value)) {}
		BTCValue(int value) : BTCValue(static_cast<unsigned long long>(value)) {}
		BTCValue(unsigned int value) : BTCValue(static_cast<unsigned long long>(value)) {}
		BTCValue(long value) : BTCValue(static_cast<unsigned long long>(value)) {}
		BTCValue(unsigned long value) : BTCValue(static_cast<unsigned long long>(value)) {}
		BTCValue(long long value) : BTCValue(static_cast<unsigned long long>(value)) {}
		BTCValue(unsigned long long value) { this->value = value; }
		BTCValue(float value, const char* unit = "sat") : BTCValue(static_cast<double>(value), unit) {}
		BTCValue(double value, const char* unit = "sat");
		BTCValue(long double value, const char* unit = "sat") : BTCValue(static_cast<double>(value), unit) {}

		BTCValue(const BTCValue& other) { this->value = other.value; }

		explicit BTCValue(const char* str);
		BTCValue(const ::omnetpp::cValue& value);
		BTCValue(const ::omnetpp::cPar& par, const char* defaultUnit = nullptr);
		BTCValue(const ::omnetpp::cProperty& prop, const char* defaultUnit = "BTC");

		BTCValue& operator=(const BTCValue& other) { if (this == &other) return *this; this->value = other.value; return *this; }

		BTCValue& operator=(const bool& value) { return *this = BTCValue(value); }
		BTCValue& operator=(const char& value) { return *this = BTCValue(value); }
		BTCValue& operator=(const unsigned char& value) { return *this = BTCValue(value); }
		BTCValue& operator=(const short& value) { return *this = BTCValue(value); }
		BTCValue& operator=(const unsigned short& value) { return *this = BTCValue(value); }
		BTCValue& operator=(const int& value) { return *this = BTCValue(value); }
		BTCValue& operator=(const unsigned int& value) { return *this = BTCValue(value); }
		BTCValue& operator=(const long& value) { return *this = BTCValue(value); }
		BTCValue& operator=(const unsigned long& value) { return *this = BTCValue(value); }
		BTCValue& operator=(const long long& value) { return *this = BTCValue(value); }
		BTCValue& operator=(const unsigned long long& value) { return *this = BTCValue(value); }
		BTCValue& operator=(const float& value) { return *this = BTCValue(value); }
		BTCValue& operator=(const double& value) { return *this = BTCValue(value); }
		BTCValue& operator=(const long double& value) { return *this = BTCValue(value); }

		BTCValue& operator=(const char* str) { return *this = BTCValue(str); }
		BTCValue& operator=(const ::omnetpp::cValue& str) { return *this = BTCValue(value); }
		BTCValue& operator=(const ::omnetpp::cPar& par) { return *this = BTCValue(par); }
		BTCValue& operator=(const ::omnetpp::cProperty& prop) { return *this = BTCValue(prop); }

		constexpr bool operator==(const BTCValue& other) const noexcept { return this->value == other.value; }
		constexpr bool operator!=(const BTCValue& other) const noexcept { return this->value != other.value; }
		constexpr bool operator<(const BTCValue& other) const noexcept { return this->value < other.value; }
		constexpr bool operator>(const BTCValue& other) const noexcept { return this->value > other.value; }
		constexpr bool operator<=(const BTCValue& other) const noexcept { return this->value <= other.value; }
		constexpr bool operator>=(const BTCValue& other) const noexcept { return this->value >= other.value; }

		const BTCValue operator+(const BTCValue& other) const { BTCValue ret; ret.value = this->value + other.value; return ret; }
		const BTCValue operator-(const BTCValue& other) const { BTCValue ret; ret.value = this->value - other.value; return ret; }
		const BTCValue operator*(const BTCValue& other) const { BTCValue ret; ret.value = this->value * other.value; return ret; }
		const BTCValue operator/(const BTCValue& other) const { BTCValue ret; ret.value = this->value / other.value; return ret; }
		const BTCValue operator%(const BTCValue& other) const { BTCValue ret; ret.value = this->value % other.value; return ret; }
		const BTCValue operator&(const BTCValue& other) const { BTCValue ret; ret.value = this->value & other.value; return ret; }
		const BTCValue operator|(const BTCValue& other) const { BTCValue ret; ret.value = this->value | other.value; return ret; }
		const BTCValue operator^(const BTCValue& other) const { BTCValue ret; ret.value = this->value ^ other.value; return ret; }

		const BTCValue operator<<(const int shift) const { BTCValue ret; ret.value = this->value << shift; return ret; }
		const BTCValue operator>>(const int shift) const { BTCValue ret; ret.value = this->value >> shift; return ret; }

		BTCValue& operator++() { ++this->value; return *this; }
		BTCValue& operator--() { --this->value; return *this; }
		const BTCValue operator++(int) { BTCValue ret = *this; ++*this; return ret; }
		const BTCValue operator--(int) { BTCValue ret = *this; --*this; return ret; }

		BTCValue& operator+=(const BTCValue& other) { return *this = *this + other; }
		BTCValue& operator-=(const BTCValue& other) { return *this = *this - other; }
		BTCValue& operator*=(const BTCValue& other) { return *this = *this * other; }
		BTCValue& operator/=(const BTCValue& other) { return *this = *this / other; }
		BTCValue& operator%=(const BTCValue& other) { return *this = *this % other; }
		BTCValue& operator&=(const BTCValue& other) { return *this = *this & other; }
		BTCValue& operator|=(const BTCValue& other) { return *this = *this | other; }
		BTCValue& operator^=(const BTCValue& other) { return *this = *this ^ other; }

		bool operator==(const bool& value) const noexcept { return value == (this->value != 0); }
		bool operator!=(const bool& value) const noexcept { return value != (this->value != 0); }

		const BTCValue operator+(const bool& value) const { return *this + BTCValue(value); }
		const BTCValue operator-(const bool& value) const { return *this - BTCValue(value); }
		const BTCValue operator*(const bool& value) const { return *this * BTCValue(value); }
		const BTCValue operator/(const bool& value) const { return *this / BTCValue(value); }
		const BTCValue operator%(const bool& value) const { return *this % BTCValue(value); }

		BTCValue& operator+=(const bool& value) { return *this = *this + BTCValue(value); }
		BTCValue& operator-=(const bool& value) { return *this = *this - BTCValue(value); }
		BTCValue& operator*=(const bool& value) { return *this = *this * BTCValue(value); }
		BTCValue& operator/=(const bool& value) { return *this = *this / BTCValue(value); }
		BTCValue& operator%=(const bool& value) { return *this = *this % BTCValue(value); }

		friend inline const BTCValue operator+(const bool a, const BTCValue& b) { return BTCValue(a) + b; }
		friend inline const BTCValue operator-(const bool a, const BTCValue& b) { return BTCValue(a) - b; }
		friend inline const BTCValue operator*(const bool a, const BTCValue& b) { return BTCValue(a) * b; }
		friend inline const BTCValue operator/(const bool a, const BTCValue& b) { return BTCValue(a) / b; }
		friend inline const BTCValue operator%(const bool a, const BTCValue& b) { return BTCValue(a) % b; }
		friend inline bool operator==(const bool a, const BTCValue& b) noexcept { return a == (b.value != 0); }
		friend inline bool operator!=(const bool a, const BTCValue& b) noexcept { return a != (b.value != 0); }

		bool operator==(const char& value) const { return *this == BTCValue(value); }
		bool operator!=(const char& value) const { return *this != BTCValue(value); }
		bool operator<(const char& value) const { return *this < BTCValue(value); }
		bool operator>(const char& value) const { return *this > BTCValue(value); }
		bool operator<=(const char& value) const { return *this <= BTCValue(value); }
		bool operator>=(const char& value) const { return *this >= BTCValue(value); }

		const BTCValue operator+(const char& value) const { return *this + BTCValue(value); }
		const BTCValue operator-(const char& value) const { return *this - BTCValue(value); }
		const BTCValue operator*(const char& value) const { return *this * BTCValue(value); }
		const BTCValue operator/(const char& value) const { return *this / BTCValue(value); }
		const BTCValue operator%(const char& value) const { return *this % BTCValue(value); }

		BTCValue& operator+=(const char& value) { return *this = *this + BTCValue(value); }
		BTCValue& operator-=(const char& value) { return *this = *this - BTCValue(value); }
		BTCValue& operator*=(const char& value) { return *this = *this * BTCValue(value); }
		BTCValue& operator/=(const char& value) { return *this = *this / BTCValue(value); }
		BTCValue& operator%=(const char& value) { return *this = *this % BTCValue(value); }

		friend inline const BTCValue operator+(const char a, const BTCValue& b) { return BTCValue(a) + b; }
		friend inline const BTCValue operator-(const char a, const BTCValue& b) { return BTCValue(a) - b; }
		friend inline const BTCValue operator*(const char a, const BTCValue& b) { return BTCValue(a) * b; }
		friend inline const BTCValue operator/(const char a, const BTCValue& b) { return BTCValue(a) / b; }
		friend inline const BTCValue operator%(const char a, const BTCValue& b) { return BTCValue(a) % b; }
		friend inline bool operator==(const char a, const BTCValue& b) { return BTCValue(a) == b; }
		friend inline bool operator!=(const char a, const BTCValue& b) { return BTCValue(a) != b; }
		friend inline bool operator<(const char a, const BTCValue& b) { return BTCValue(a) < b; }
		friend inline bool operator>(const char a, const BTCValue& b) { return BTCValue(a) > b; }
		friend inline bool operator<=(const char a, const BTCValue& b) { return BTCValue(a) <= b; }
		friend inline bool operator>=(const char a, const BTCValue& b) { return BTCValue(a) >= b; }

		bool operator==(const unsigned char& value) const { return *this == BTCValue(value); }
		bool operator!=(const unsigned char& value) const { return *this != BTCValue(value); }
		bool operator<(const unsigned char& value) const { return *this < BTCValue(value); }
		bool operator>(const unsigned char& value) const { return *this > BTCValue(value); }
		bool operator<=(const unsigned char& value) const { return *this <= BTCValue(value); }
		bool operator>=(const unsigned char& value) const { return *this >= BTCValue(value); }

		const BTCValue operator+(const unsigned char& value) const { return *this + BTCValue(value); }
		const BTCValue operator-(const unsigned char& value) const { return *this - BTCValue(value); }
		const BTCValue operator*(const unsigned char& value) const { return *this * BTCValue(value); }
		const BTCValue operator/(const unsigned char& value) const { return *this / BTCValue(value); }
		const BTCValue operator%(const unsigned char& value) const { return *this % BTCValue(value); }

		BTCValue& operator+=(const unsigned char& value) { return *this = *this + BTCValue(value); }
		BTCValue& operator-=(const unsigned char& value) { return *this = *this - BTCValue(value); }
		BTCValue& operator*=(const unsigned char& value) { return *this = *this * BTCValue(value); }
		BTCValue& operator/=(const unsigned char& value) { return *this = *this / BTCValue(value); }
		BTCValue& operator%=(const unsigned char& value) { return *this = *this % BTCValue(value); }

		friend inline const BTCValue operator+(const unsigned char a, const BTCValue& b) { return BTCValue(a) + b; }
		friend inline const BTCValue operator-(const unsigned char a, const BTCValue& b) { return BTCValue(a) - b; }
		friend inline const BTCValue operator*(const unsigned char a, const BTCValue& b) { return BTCValue(a) * b; }
		friend inline const BTCValue operator/(const unsigned char a, const BTCValue& b) { return BTCValue(a) / b; }
		friend inline const BTCValue operator%(const unsigned char a, const BTCValue& b) { return BTCValue(a) % b; }
		friend inline bool operator==(const unsigned char a, const BTCValue& b) { return BTCValue(a) == b; }
		friend inline bool operator!=(const unsigned char a, const BTCValue& b) { return BTCValue(a) != b; }
		friend inline bool operator<(const unsigned char a, const BTCValue& b) { return BTCValue(a) < b; }
		friend inline bool operator>(const unsigned char a, const BTCValue& b) { return BTCValue(a) > b; }
		friend inline bool operator<=(const unsigned char a, const BTCValue& b) { return BTCValue(a) <= b; }
		friend inline bool operator>=(const unsigned char a, const BTCValue& b) { return BTCValue(a) >= b; }

		bool operator==(const short& value) const { return *this == BTCValue(value); }
		bool operator!=(const short& value) const { return *this != BTCValue(value); }
		bool operator<(const short& value) const { return *this < BTCValue(value); }
		bool operator>(const short& value) const { return *this > BTCValue(value); }
		bool operator<=(const short& value) const { return *this <= BTCValue(value); }
		bool operator>=(const short& value) const { return *this >= BTCValue(value); }

		const BTCValue operator+(const short& value) const { return *this + BTCValue(value); }
		const BTCValue operator-(const short& value) const { return *this - BTCValue(value); }
		const BTCValue operator*(const short& value) const { return *this * BTCValue(value); }
		const BTCValue operator/(const short& value) const { return *this / BTCValue(value); }
		const BTCValue operator%(const short& value) const { return *this % BTCValue(value); }

		BTCValue& operator+=(const short& value) { return *this = *this + BTCValue(value); }
		BTCValue& operator-=(const short& value) { return *this = *this - BTCValue(value); }
		BTCValue& operator*=(const short& value) { return *this = *this * BTCValue(value); }
		BTCValue& operator/=(const short& value) { return *this = *this / BTCValue(value); }
		BTCValue& operator%=(const short& value) { return *this = *this % BTCValue(value); }

		friend inline const BTCValue operator+(const short a, const BTCValue& b) { return BTCValue(a) + b; }
		friend inline const BTCValue operator-(const short a, const BTCValue& b) { return BTCValue(a) - b; }
		friend inline const BTCValue operator*(const short a, const BTCValue& b) { return BTCValue(a) * b; }
		friend inline const BTCValue operator/(const short a, const BTCValue& b) { return BTCValue(a) / b; }
		friend inline const BTCValue operator%(const short a, const BTCValue& b) { return BTCValue(a) % b; }
		friend inline bool operator==(const short a, const BTCValue& b) { return BTCValue(a) == b; }
		friend inline bool operator!=(const short a, const BTCValue& b) { return BTCValue(a) != b; }
		friend inline bool operator<(const short a, const BTCValue& b) { return BTCValue(a) < b; }
		friend inline bool operator>(const short a, const BTCValue& b) { return BTCValue(a) > b; }
		friend inline bool operator<=(const short a, const BTCValue& b) { return BTCValue(a) <= b; }
		friend inline bool operator>=(const short a, const BTCValue& b) { return BTCValue(a) >= b; }

		bool operator==(const unsigned short& value) const { return *this == BTCValue(value); }
		bool operator!=(const unsigned short& value) const { return *this != BTCValue(value); }
		bool operator<(const unsigned short& value) const { return *this < BTCValue(value); }
		bool operator>(const unsigned short& value) const { return *this > BTCValue(value); }
		bool operator<=(const unsigned short& value) const { return *this <= BTCValue(value); }
		bool operator>=(const unsigned short& value) const { return *this >= BTCValue(value); }

		const BTCValue operator+(const unsigned short& value) const { return *this + BTCValue(value); }
		const BTCValue operator-(const unsigned short& value) const { return *this - BTCValue(value); }
		const BTCValue operator*(const unsigned short& value) const { return *this * BTCValue(value); }
		const BTCValue operator/(const unsigned short& value) const { return *this / BTCValue(value); }
		const BTCValue operator%(const unsigned short& value) const { return *this % BTCValue(value); }

		BTCValue& operator+=(const unsigned short& value) { return *this = *this + BTCValue(value); }
		BTCValue& operator-=(const unsigned short& value) { return *this = *this - BTCValue(value); }
		BTCValue& operator*=(const unsigned short& value) { return *this = *this * BTCValue(value); }
		BTCValue& operator/=(const unsigned short& value) { return *this = *this / BTCValue(value); }
		BTCValue& operator%=(const unsigned short& value) { return *this = *this % BTCValue(value); }

		friend inline const BTCValue operator+(const unsigned short a, const BTCValue& b) { return BTCValue(a) + b; }
		friend inline const BTCValue operator-(const unsigned short a, const BTCValue& b) { return BTCValue(a) - b; }
		friend inline const BTCValue operator*(const unsigned short a, const BTCValue& b) { return BTCValue(a) * b; }
		friend inline const BTCValue operator/(const unsigned short a, const BTCValue& b) { return BTCValue(a) / b; }
		friend inline const BTCValue operator%(const unsigned short a, const BTCValue& b) { return BTCValue(a) % b; }
		friend inline bool operator==(const unsigned short a, const BTCValue& b) { return BTCValue(a) == b; }
		friend inline bool operator!=(const unsigned short a, const BTCValue& b) { return BTCValue(a) != b; }
		friend inline bool operator<(const unsigned short a, const BTCValue& b) { return BTCValue(a) < b; }
		friend inline bool operator>(const unsigned short a, const BTCValue& b) { return BTCValue(a) > b; }
		friend inline bool operator<=(const unsigned short a, const BTCValue& b) { return BTCValue(a) <= b; }
		friend inline bool operator>=(const unsigned short a, const BTCValue& b) { return BTCValue(a) >= b; }

		bool operator==(const int& value) const { return *this == BTCValue(value); }
		bool operator!=(const int& value) const { return *this != BTCValue(value); }
		bool operator<(const int& value) const { return *this < BTCValue(value); }
		bool operator>(const int& value) const { return *this > BTCValue(value); }
		bool operator<=(const int& value) const { return *this <= BTCValue(value); }
		bool operator>=(const int& value) const { return *this >= BTCValue(value); }

		const BTCValue operator+(const int& value) const { return *this + BTCValue(value); }
		const BTCValue operator-(const int& value) const { return *this - BTCValue(value); }
		const BTCValue operator*(const int& value) const { return *this * BTCValue(value); }
		const BTCValue operator/(const int& value) const { return *this / BTCValue(value); }
		const BTCValue operator%(const int& value) const { return *this % BTCValue(value); }

		BTCValue& operator+=(const int& value) { return *this = *this + BTCValue(value); }
		BTCValue& operator-=(const int& value) { return *this = *this - BTCValue(value); }
		BTCValue& operator*=(const int& value) { return *this = *this * BTCValue(value); }
		BTCValue& operator/=(const int& value) { return *this = *this / BTCValue(value); }
		BTCValue& operator%=(const int& value) { return *this = *this % BTCValue(value); }

		friend inline const BTCValue operator+(const int a, const BTCValue& b) { return BTCValue(a) + b; }
		friend inline const BTCValue operator-(const int a, const BTCValue& b) { return BTCValue(a) - b; }
		friend inline const BTCValue operator*(const int a, const BTCValue& b) { return BTCValue(a) * b; }
		friend inline const BTCValue operator/(const int a, const BTCValue& b) { return BTCValue(a) / b; }
		friend inline const BTCValue operator%(const int a, const BTCValue& b) { return BTCValue(a) % b; }
		friend inline bool operator==(const int a, const BTCValue& b) { return BTCValue(a) == b; }
		friend inline bool operator!=(const int a, const BTCValue& b) { return BTCValue(a) != b; }
		friend inline bool operator<(const int a, const BTCValue& b) { return BTCValue(a) < b; }
		friend inline bool operator>(const int a, const BTCValue& b) { return BTCValue(a) > b; }
		friend inline bool operator<=(const int a, const BTCValue& b) { return BTCValue(a) <= b; }
		friend inline bool operator>=(const int a, const BTCValue& b) { return BTCValue(a) >= b; }

		bool operator==(const unsigned int& value) const { return *this == BTCValue(value); }
		bool operator!=(const unsigned int& value) const { return *this != BTCValue(value); }
		bool operator<(const unsigned int& value) const { return *this < BTCValue(value); }
		bool operator>(const unsigned int& value) const { return *this > BTCValue(value); }
		bool operator<=(const unsigned int& value) const { return *this <= BTCValue(value); }
		bool operator>=(const unsigned int& value) const { return *this >= BTCValue(value); }

		const BTCValue operator+(const unsigned int& value) const { return *this + BTCValue(value); }
		const BTCValue operator-(const unsigned int& value) const { return *this - BTCValue(value); }
		const BTCValue operator*(const unsigned int& value) const { return *this * BTCValue(value); }
		const BTCValue operator/(const unsigned int& value) const { return *this / BTCValue(value); }
		const BTCValue operator%(const unsigned int& value) const { return *this % BTCValue(value); }

		BTCValue& operator+=(const unsigned int& value) { return *this = *this + BTCValue(value); }
		BTCValue& operator-=(const unsigned int& value) { return *this = *this - BTCValue(value); }
		BTCValue& operator*=(const unsigned int& value) { return *this = *this * BTCValue(value); }
		BTCValue& operator/=(const unsigned int& value) { return *this = *this / BTCValue(value); }
		BTCValue& operator%=(const unsigned int& value) { return *this = *this % BTCValue(value); }

		friend inline const BTCValue operator+(const unsigned int a, const BTCValue& b) { return BTCValue(a) + b; }
		friend inline const BTCValue operator-(const unsigned int a, const BTCValue& b) { return BTCValue(a) - b; }
		friend inline const BTCValue operator*(const unsigned int a, const BTCValue& b) { return BTCValue(a) * b; }
		friend inline const BTCValue operator/(const unsigned int a, const BTCValue& b) { return BTCValue(a) / b; }
		friend inline const BTCValue operator%(const unsigned int a, const BTCValue& b) { return BTCValue(a) % b; }
		friend inline bool operator==(const unsigned int a, const BTCValue& b) { return BTCValue(a) == b; }
		friend inline bool operator!=(const unsigned int a, const BTCValue& b) { return BTCValue(a) != b; }
		friend inline bool operator<(const unsigned int a, const BTCValue& b) { return BTCValue(a) < b; }
		friend inline bool operator>(const unsigned int a, const BTCValue& b) { return BTCValue(a) > b; }
		friend inline bool operator<=(const unsigned int a, const BTCValue& b) { return BTCValue(a) <= b; }
		friend inline bool operator>=(const unsigned int a, const BTCValue& b) { return BTCValue(a) >= b; }

		bool operator==(const long& value) const { return *this == BTCValue(value); }
		bool operator!=(const long& value) const { return *this != BTCValue(value); }
		bool operator<(const long& value) const { return *this < BTCValue(value); }
		bool operator>(const long& value) const { return *this > BTCValue(value); }
		bool operator<=(const long& value) const { return *this <= BTCValue(value); }
		bool operator>=(const long& value) const { return *this >= BTCValue(value); }

		const BTCValue operator+(const long& value) const { return *this + BTCValue(value); }
		const BTCValue operator-(const long& value) const { return *this - BTCValue(value); }
		const BTCValue operator*(const long& value) const { return *this * BTCValue(value); }
		const BTCValue operator/(const long& value) const { return *this / BTCValue(value); }
		const BTCValue operator%(const long& value) const { return *this % BTCValue(value); }

		BTCValue& operator+=(const long& value) { return *this = *this + BTCValue(value); }
		BTCValue& operator-=(const long& value) { return *this = *this - BTCValue(value); }
		BTCValue& operator*=(const long& value) { return *this = *this * BTCValue(value); }
		BTCValue& operator/=(const long& value) { return *this = *this / BTCValue(value); }
		BTCValue& operator%=(const long& value) { return *this = *this % BTCValue(value); }

		friend inline const BTCValue operator+(const long a, const BTCValue& b) { return BTCValue(a) + b; }
		friend inline const BTCValue operator-(const long a, const BTCValue& b) { return BTCValue(a) - b; }
		friend inline const BTCValue operator*(const long a, const BTCValue& b) { return BTCValue(a) * b; }
		friend inline const BTCValue operator/(const long a, const BTCValue& b) { return BTCValue(a) / b; }
		friend inline const BTCValue operator%(const long a, const BTCValue& b) { return BTCValue(a) % b; }
		friend inline bool operator==(const long a, const BTCValue& b) { return BTCValue(a) == b; }
		friend inline bool operator!=(const long a, const BTCValue& b) { return BTCValue(a) != b; }
		friend inline bool operator<(const long a, const BTCValue& b) { return BTCValue(a) < b; }
		friend inline bool operator>(const long a, const BTCValue& b) { return BTCValue(a) > b; }
		friend inline bool operator<=(const long a, const BTCValue& b) { return BTCValue(a) <= b; }
		friend inline bool operator>=(const long a, const BTCValue& b) { return BTCValue(a) >= b; }

		bool operator==(const unsigned long& value) const { return *this == BTCValue(value); }
		bool operator!=(const unsigned long& value) const { return *this != BTCValue(value); }
		bool operator<(const unsigned long& value) const { return *this < BTCValue(value); }
		bool operator>(const unsigned long& value) const { return *this > BTCValue(value); }
		bool operator<=(const unsigned long& value) const { return *this <= BTCValue(value); }
		bool operator>=(const unsigned long& value) const { return *this >= BTCValue(value); }

		const BTCValue operator+(const unsigned long& value) const { return *this + BTCValue(value); }
		const BTCValue operator-(const unsigned long& value) const { return *this - BTCValue(value); }
		const BTCValue operator*(const unsigned long& value) const { return *this * BTCValue(value); }
		const BTCValue operator/(const unsigned long& value) const { return *this / BTCValue(value); }
		const BTCValue operator%(const unsigned long& value) const { return *this % BTCValue(value); }

		BTCValue& operator+=(const unsigned long& value) { return *this = *this + BTCValue(value); }
		BTCValue& operator-=(const unsigned long& value) { return *this = *this - BTCValue(value); }
		BTCValue& operator*=(const unsigned long& value) { return *this = *this * BTCValue(value); }
		BTCValue& operator/=(const unsigned long& value) { return *this = *this / BTCValue(value); }
		BTCValue& operator%=(const unsigned long& value) { return *this = *this % BTCValue(value); }

		friend inline const BTCValue operator+(const unsigned long a, const BTCValue& b) { return BTCValue(a) + b; }
		friend inline const BTCValue operator-(const unsigned long a, const BTCValue& b) { return BTCValue(a) - b; }
		friend inline const BTCValue operator*(const unsigned long a, const BTCValue& b) { return BTCValue(a) * b; }
		friend inline const BTCValue operator/(const unsigned long a, const BTCValue& b) { return BTCValue(a) / b; }
		friend inline const BTCValue operator%(const unsigned long a, const BTCValue& b) { return BTCValue(a) % b; }
		friend inline bool operator==(const unsigned long a, const BTCValue& b) { return BTCValue(a) == b; }
		friend inline bool operator!=(const unsigned long a, const BTCValue& b) { return BTCValue(a) != b; }
		friend inline bool operator<(const unsigned long a, const BTCValue& b) { return BTCValue(a) < b; }
		friend inline bool operator>(const unsigned long a, const BTCValue& b) { return BTCValue(a) > b; }
		friend inline bool operator<=(const unsigned long a, const BTCValue& b) { return BTCValue(a) <= b; }
		friend inline bool operator>=(const unsigned long a, const BTCValue& b) { return BTCValue(a) >= b; }

		bool operator==(const long long& value) const { return *this == BTCValue(value); }
		bool operator!=(const long long& value) const { return *this != BTCValue(value); }
		bool operator<(const long long& value) const { return *this < BTCValue(value); }
		bool operator>(const long long& value) const { return *this > BTCValue(value); }
		bool operator<=(const long long& value) const { return *this <= BTCValue(value); }
		bool operator>=(const long long& value) const { return *this >= BTCValue(value); }

		const BTCValue operator+(const long long& value) const { return *this + BTCValue(value); }
		const BTCValue operator-(const long long& value) const { return *this - BTCValue(value); }
		const BTCValue operator*(const long long& value) const { return *this * BTCValue(value); }
		const BTCValue operator/(const long long& value) const { return *this / BTCValue(value); }
		const BTCValue operator%(const long long& value) const { return *this % BTCValue(value); }

		BTCValue& operator+=(const long long& value) { return *this = *this + BTCValue(value); }
		BTCValue& operator-=(const long long& value) { return *this = *this - BTCValue(value); }
		BTCValue& operator*=(const long long& value) { return *this = *this * BTCValue(value); }
		BTCValue& operator/=(const long long& value) { return *this = *this / BTCValue(value); }
		friend inline bool operator<(const bool a, const BTCValue& b) { return BTCValue(a) < b; }
		friend inline bool operator>(const bool a, const BTCValue& b) { return BTCValue(a) > b; }
		friend inline bool operator<=(const bool a, const BTCValue& b) { return BTCValue(a) <= b; }
		friend inline bool operator>=(const bool a, const BTCValue& b) { return BTCValue(a) >= b; }
		BTCValue& operator%=(const long long& value) { return *this = *this % BTCValue(value); }

		friend inline const BTCValue operator+(const long long a, const BTCValue& b) { return BTCValue(a) + b; }
		friend inline const BTCValue operator-(const long long a, const BTCValue& b) { return BTCValue(a) - b; }
		friend inline const BTCValue operator*(const long long a, const BTCValue& b) { return BTCValue(a) * b; }
		friend inline const BTCValue operator/(const long long a, const BTCValue& b) { return BTCValue(a) / b; }
		friend inline const BTCValue operator%(const long long a, const BTCValue& b) { return BTCValue(a) % b; }
		friend inline bool operator==(const long long a, const BTCValue& b) { return BTCValue(a) == b; }
		friend inline bool operator!=(const long long a, const BTCValue& b) { return BTCValue(a) != b; }
		friend inline bool operator<(const long long a, const BTCValue& b) { return BTCValue(a) < b; }
		friend inline bool operator>(const long long a, const BTCValue& b) { return BTCValue(a) > b; }
		friend inline bool operator<=(const long long a, const BTCValue& b) { return BTCValue(a) <= b; }
		friend inline bool operator>=(const long long a, const BTCValue& b) { return BTCValue(a) >= b; }

		bool operator==(const unsigned long long& value) const { return *this == BTCValue(value); }
		bool operator!=(const unsigned long long& value) const { return *this != BTCValue(value); }
		bool operator<(const unsigned long long& value) const { return *this < BTCValue(value); }
		bool operator>(const unsigned long long& value) const { return *this > BTCValue(value); }
		bool operator<=(const unsigned long long& value) const { return *this <= BTCValue(value); }
		bool operator>=(const unsigned long long& value) const { return *this >= BTCValue(value); }

		const BTCValue operator+(const unsigned long long& value) const { return *this + BTCValue(value); }
		const BTCValue operator-(const unsigned long long& value) const { return *this - BTCValue(value); }
		const BTCValue operator*(const unsigned long long& value) const { return *this * BTCValue(value); }
		const BTCValue operator/(const unsigned long long& value) const { return *this / BTCValue(value); }
		const BTCValue operator%(const unsigned long long& value) const { return *this % BTCValue(value); }

		BTCValue& operator+=(const unsigned long long& value) { return *this = *this + BTCValue(value); }
		BTCValue& operator-=(const unsigned long long& value) { return *this = *this - BTCValue(value); }
		BTCValue& operator*=(const unsigned long long& value) { return *this = *this * BTCValue(value); }
		BTCValue& operator/=(const unsigned long long& value) { return *this = *this / BTCValue(value); }
		BTCValue& operator%=(const unsigned long long& value) { return *this = *this % BTCValue(value); }

		friend inline const BTCValue operator+(const unsigned long long a, const BTCValue& b) { return BTCValue(a) + b; }
		friend inline const BTCValue operator-(const unsigned long long a, const BTCValue& b) { return BTCValue(a) - b; }
		friend inline const BTCValue operator*(const unsigned long long a, const BTCValue& b) { return BTCValue(a) * b; }
		friend inline const BTCValue operator/(const unsigned long long a, const BTCValue& b) { return BTCValue(a) / b; }
		friend inline const BTCValue operator%(const unsigned long long a, const BTCValue& b) { return BTCValue(a) % b; }
		friend inline bool operator==(const unsigned long long a, const BTCValue& b) { return BTCValue(a) == b; }
		friend inline bool operator!=(const unsigned long long a, const BTCValue& b) { return BTCValue(a) != b; }
		friend inline bool operator<(const unsigned long long a, const BTCValue& b) { return BTCValue(a) < b; }
		friend inline bool operator>(const unsigned long long a, const BTCValue& b) { return BTCValue(a) > b; }
		friend inline bool operator<=(const unsigned long long a, const BTCValue& b) { return BTCValue(a) <= b; }
		friend inline bool operator>=(const unsigned long long a, const BTCValue& b) { return BTCValue(a) >= b; }

		bool operator==(const double& value) const { return *this == BTCValue(value); }
		bool operator!=(const double& value) const { return *this != BTCValue(value); }
		bool operator<(const double& value) const { return *this < BTCValue(value); }
		bool operator>(const double& value) const { return *this > BTCValue(value); }
		bool operator<=(const double& value) const { return *this <= BTCValue(value); }
		bool operator>=(const double& value) const { return *this >= BTCValue(value); }

		const BTCValue operator+(const double& value) const { return *this + BTCValue(value); }
		const BTCValue operator-(const double& value) const { return *this - BTCValue(value); }
		const BTCValue operator*(const double& value) const { return *this * BTCValue(value); }
		const BTCValue operator/(const double& value) const { return *this / BTCValue(value); }
		const BTCValue operator%(const double& value) const { return *this % BTCValue(value); }

		BTCValue& operator+=(const double& value) { return *this = *this + BTCValue(value); }
		BTCValue& operator-=(const double& value) { return *this = *this - BTCValue(value); }
		BTCValue& operator*=(const double& value) { return *this = *this * BTCValue(value); }
		BTCValue& operator/=(const double& value) { return *this = *this / BTCValue(value); }
		BTCValue& operator%=(const double& value) { return *this = *this % BTCValue(value); }

		friend inline const BTCValue operator+(const double a, const BTCValue& b) { return BTCValue(a) + b; }
		friend inline const BTCValue operator-(const double a, const BTCValue& b) { return BTCValue(a) - b; }
		friend inline const BTCValue operator*(const double a, const BTCValue& b) { return BTCValue(a) * b; }
		friend inline const BTCValue operator/(const double a, const BTCValue& b) { return BTCValue(a) / b; }
		friend inline const BTCValue operator%(const double a, const BTCValue& b) { return BTCValue(a) % b; }
		friend inline bool operator==(const double a, const BTCValue& b) { return BTCValue(a) == b; }
		friend inline bool operator!=(const double a, const BTCValue& b) { return BTCValue(a) != b; }
		friend inline bool operator<(const double a, const BTCValue& b) { return BTCValue(a) < b; }
		friend inline bool operator>(const double a, const BTCValue& b) { return BTCValue(a) > b; }
		friend inline bool operator<=(const double a, const BTCValue& b) { return BTCValue(a) <= b; }
		friend inline bool operator>=(const double a, const BTCValue& b) { return BTCValue(a) >= b; }

		bool operator==(const long double& value) const { return *this == BTCValue(value); }
		bool operator!=(const long double& value) const { return *this != BTCValue(value); }
		bool operator<(const long double& value) const { return *this < BTCValue(value); }
		bool operator>(const long double& value) const { return *this > BTCValue(value); }
		bool operator<=(const long double& value) const { return *this <= BTCValue(value); }
		bool operator>=(const long double& value) const { return *this >= BTCValue(value); }

		const BTCValue operator+(const long double& value) const { return *this + BTCValue(value); }
		const BTCValue operator-(const long double& value) const { return *this - BTCValue(value); }
		const BTCValue operator*(const long double& value) const { return *this * BTCValue(value); }
		const BTCValue operator/(const long double& value) const { return *this / BTCValue(value); }
		const BTCValue operator%(const long double& value) const { return *this % BTCValue(value); }

		BTCValue& operator+=(const long double& value) { return *this = *this + BTCValue(value); }
		BTCValue& operator-=(const long double& value) { return *this = *this - BTCValue(value); }
		BTCValue& operator*=(const long double& value) { return *this = *this * BTCValue(value); }
		BTCValue& operator/=(const long double& value) { return *this = *this / BTCValue(value); }
		BTCValue& operator%=(const long double& value) { return *this = *this % BTCValue(value); }

		friend inline const BTCValue operator+(const long double a, const BTCValue& b) { return BTCValue(a) + b; }
		friend inline const BTCValue operator-(const long double a, const BTCValue& b) { return BTCValue(a) - b; }
		friend inline const BTCValue operator*(const long double a, const BTCValue& b) { return BTCValue(a) * b; }
		friend inline const BTCValue operator/(const long double a, const BTCValue& b) { return BTCValue(a) / b; }
		friend inline const BTCValue operator%(const long double a, const BTCValue& b) { return BTCValue(a) % b; }
		friend inline bool operator==(const long double a, const BTCValue& b) { return BTCValue(a) == b; }
		friend inline bool operator!=(const long double a, const BTCValue& b) { return BTCValue(a) != b; }
		friend inline bool operator<(const long double a, const BTCValue& b) { return BTCValue(a) < b; }
		friend inline bool operator>(const long double a, const BTCValue& b) { return BTCValue(a) > b; }
		friend inline bool operator<=(const long double a, const BTCValue& b) { return BTCValue(a) <= b; }
		friend inline bool operator>=(const long double a, const BTCValue& b) { return BTCValue(a) >= b; }

		constexpr double TAM() const { return convert(value, "sat", "TAM"); }
		constexpr double tam() const { return convert(value, "sat", "tam"); }
		constexpr double MBTC() const { return convert(value, "sat", "MBTC"); }
		constexpr double kBTC() const { return convert(value, "sat", "kBTC"); }
		constexpr double hBTC() const { return convert(value, "sat", "hBTC"); }
		constexpr double bTBC() const { return convert(value, "sat", "bTBC"); }
		constexpr double daBTC() const { return convert(value, "sat", "daBTC"); }
		constexpr double mTBC() const { return convert(value, "sat", "mTBC"); }
		constexpr double BTC() const { return convert(value, "sat", "BTC"); }
		constexpr double btc() const { return convert(value, "sat", "btc"); }
		constexpr double dBTC() const { return convert(value, "sat", "dBTC"); }
		constexpr double tTBC() const { return convert(value, "sat", "tTBC"); }
		constexpr double cBTC() const { return convert(value, "sat", "cBTC"); }
		constexpr double mBTC() const { return convert(value, "sat", "mBTC"); }
		constexpr double TBC() const { return convert(value, "sat", "TBC"); }
		constexpr double tbc() const { return convert(value, "sat", "tbc"); }
		constexpr double TBCt() const { return convert(value, "sat", "TBCt"); }
		constexpr double TBCs() const { return convert(value, "sat", "TBCs"); }
		constexpr double uBTC() const { return convert(value, "sat", "uBTC"); }
		constexpr double TBCm() const { return convert(value, "sat", "TBCm"); }
		constexpr double finney() const { return convert(value, "sat", "fin"); }
		constexpr double fin() const { return convert(value, "sat", "fin"); }
		constexpr double FIN() const { return convert(value, "sat", "FIN"); }
		constexpr unsigned long long TBCb() const noexcept { return value; }
		constexpr unsigned long long SAT() const noexcept { return value; }
		constexpr unsigned long long sat() const noexcept { return value; }
		constexpr unsigned long long mSAT() const { return value*1000; }
		constexpr unsigned long long msat() const { return value*1000; }
		double getValue(const char* unit = "sat") const { return convert(value, "sat", unit); }
		constexpr ::omnetpp::intval_t toValue() const { return static_cast<::omnetpp::intval_t>(value); }
		double val(const char* unit = "sat") const { return getValue(unit); }

		constexpr bool isZero() const noexcept { return value == 0; }

		constexpr operator bool() const noexcept { return value != 0; }
		constexpr operator char() const { return static_cast<char>(sat()); }
		constexpr operator unsigned char() const { return static_cast<unsigned char>(sat()); }
		constexpr operator int() const { return static_cast<int>(sat()); }
		constexpr operator unsigned int() const { return static_cast<unsigned int>(sat()); }
		constexpr operator short() const { return static_cast<short>(sat()); }
		constexpr operator unsigned short() const { return static_cast<unsigned short>(sat()); }
		constexpr operator long() const { return static_cast<long>(sat()); }
		constexpr operator unsigned long() const { return static_cast<unsigned long>(sat()); }
		constexpr operator long long() const { return static_cast<long long>(sat()); }
		constexpr operator unsigned long long() const noexcept { return sat(); }
		constexpr operator float() const { return static_cast<float>(sat()); }
		constexpr operator double() const { return static_cast<double>(sat()); }
		constexpr operator long double() const { return static_cast<long double>(sat()); }

		operator char*() const { std::string s = str(); char* ret = new char[s.size() + 1]; strcpy(ret, s.c_str()); return ret; }
		operator std::string() const { return str(); }
		std::string str() const { std::stringstream ss; ss << this->value; return ss.str(); }
};

typedef BTCValue btcvalue_t;
typedef BTCValue btcval_t;
typedef BTCValue satoshi_t;

}
}

#endif
