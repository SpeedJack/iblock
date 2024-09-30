#include "Hash.h"
#include <iomanip>

namespace iblock
{
namespace bitcoin
{

Register_Class(Hash);

void Hash::copy(const Hash& other)
{
	if (other.compact) {
		storedHash.compact = other.storedHash.compact;
	} else {
		storedHash.bytes = new unsigned char[0x20];
		std::memcpy(storedHash.bytes, other.storedHash.bytes, 0x20);
	}
	compact = other.compact;
}

Hash::Hash(uint32_t mantissa, unsigned char exponent)
{
	if (exponent > 0x20 || (mantissa & 0xFF000000) != 0 ||
			(exponent == 0x20 && mantissa & 0x800000))
		throw ::omnetpp::cRuntimeError("Value too big");

	if (mantissa & 0x800000) {
		mantissa >>= 8;
		exponent++;
	}
	if (exponent == 0 || mantissa == 0 ||
			(exponent == 1 && (mantissa & 0xFF0000) == 0) ||
			(exponent == 2 && (mantissa & 0xFFFF00) == 0))
		exponent = mantissa = 0;
	storedHash.compact = (exponent << 24) | (mantissa & 0xFFFFFF);
	compact = true;
}

Hash::Hash(const unsigned char* bytes)
{
	storedHash.bytes = new unsigned char[0x20];
	std::memcpy(storedHash.bytes, bytes, 0x20);
	compact = false;
}

Hash Hash::fromBigDouble(const double value)
{
	unsigned char exponent = static_cast<unsigned char>(std::round((std::log2(value) - 16) / 8));
	uint32_t mantissa = static_cast<uint32_t>(value / std::pow(2, exponent*8));
	return Hash(mantissa, exponent + 3);
}

Hash Hash::fromBytes(const unsigned char* bytes, size_t size)
{
	if (size > 0x20)
		size = 0x20;
	if (size == 0x20)
		return Hash(bytes);
	unsigned char hash[0x20] = { 0 };
	std::memcpy(hash + (0x20 - size), bytes, size);
	return Hash(hash);
}

Hash Hash::fromHex(const char* hex)
{
	unsigned char hash[0x20] = { 0 };
	size_t size = 0;
	if (hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X'))
		hex += 2;
	if (strlen(hex) % 2 == 1) {
		hash[0] = hexNibble(hex[0]);
		if (hash[0] < 0)
			throw ::omnetpp::cRuntimeError("Invalid hex string");
		hex++;
		size++;
	}
	for (const char* p = hex; *p != '\0' && size < 0x20; p += 2) {
		char hi = hexNibble(p[0]);
		hash[size] = hexNibble(p[1]);
		if (hi < 0 || hash[size] < 0)
			throw ::omnetpp::cRuntimeError("Invalid hex string");
		hash[size] |= hi << 4;
		size++;
	}
	return fromBytes(hash, size);
}

unsigned char* Hash::bytesCopy() const
{
	unsigned char* bytes = new unsigned char[0x20];
	if (!compact) {
		std::memcpy(bytes, storedHash.bytes, 0x20);
		return bytes;
	}
	unsigned char exponent = storedHash.compact >> 24;
	uint32_t mantissa = storedHash.compact & 0xFFFFFF;
	std::memset(bytes, 0, 0x20);
	if (exponent > 0)
		bytes[0x20 - exponent] = mantissa >> 16;
	if (exponent > 1)
		bytes[0x20 - exponent + 1] = mantissa >> 8;
	if (exponent > 2)
		bytes[0x20 - exponent + 2] = mantissa & 0xFF;
	return bytes;
}

void Hash::expand()
{
	if (!compact)
		return;
	uint32_t mantissa = storedHash.compact & 0xFFFFFF;
	unsigned char exponent = storedHash.compact >> 24;
	compact = false;
	storedHash.bytes = new unsigned char[0x20];
	std::memset(storedHash.bytes, 0, 0x20);
	if (exponent > 0)
		storedHash.bytes[0x20 - exponent] = mantissa >> 16;
	if (exponent > 1)
		storedHash.bytes[0x20 - exponent + 1] = mantissa >> 8;
	if (exponent > 2)
		storedHash.bytes[0x20 - exponent + 2] = mantissa & 0xFF;
}

uint32_t Hash::m() const
{
	if (compact)
		return storedHash.compact & 0xFFFFFF;
	unsigned char exponent = e();
	uint32_t mantissa = 0;
	if (exponent > 0)
		mantissa = storedHash.bytes[0x20 - exponent] << 16;
	if (exponent > 1)
		mantissa |= storedHash.bytes[0x20 - exponent + 1] << 8;
	if (exponent > 2)
		mantissa |= storedHash.bytes[0x20 - exponent + 2];
	return mantissa;
}

unsigned char Hash::e() const
{
	if (compact) {
		unsigned char res = storedHash.compact >> 24;
		return res;
	}
	size_t i;
	for (i = 0; i < 0x20; ++i)
		if (storedHash.bytes[i] != 0)
			break;
	if (i < 0x20 && (storedHash.bytes[i] & 0x80) != 0)
		i--;
	return 0x20 - i;
}

uint32_t Hash::compactNBits() const
{
	if (compact)
		return storedHash.compact;
	return (e() << 24) | m();
}

bool Hash::isCompactPrecise() const
{
	if (compact)
		return true;

	for (size_t i = e() + 3; i < 0x20; ++i)
		if (storedHash.bytes[i] != 0)
			return false;
	return true;
}

void Hash::makeCompact()
{
	if (compact)
		return;
	unsigned char exp = e();
	uint32_t man = m();
	delete[] storedHash.bytes;
	storedHash.compact = (exp << 24) | man;
	compact = true;
}

int32_t Hash::compare(const Hash& other) const
{
	unsigned char exp = e();
	unsigned char otherExp = other.e();
	if (exp > otherExp + 2)
		return exp - otherExp - 2;
	if (exp < otherExp - 2)
		return exp - otherExp + 2;
	if (exp == 0 && otherExp == 0)
		return 0;
	uint32_t man = m();
	uint32_t otherMan = other.m();
	if (man == 0 && otherMan == 0)
		return 0;
	if (exp == otherExp && (man != otherMan || (compact && other.compact)))
		return man - otherMan;

	unsigned char* bytes = compact ? bytesCopy() : storedHash.bytes;
	unsigned char* otherBytes = other.compact ? other.bytesCopy() : other.storedHash.bytes;
	int32_t result = 0;
	for (size_t i = 0; i < 0x20; ++i)
		if (bytes[i] - otherBytes[i] != 0) {
			result = bytes[i] - otherBytes[i];
			break;
		}

	if (compact)
		delete[] bytes;
	if (other.compact)
		delete[] otherBytes;
	return result;
}

std::string Hash::hex() const
{
	std::stringstream ss;
	unsigned char* bytes = compact ? bytesCopy() : storedHash.bytes;
	ss << "0x" << std::hex << std::setfill('0');
	for (size_t i = 0; i < 0x20; ++i)
		ss << std::setw(2) << static_cast<unsigned int>(bytes[i]);
	if (compact)
		delete[] bytes;
	return ss.str();
}

std::string Hash::str() const
{
	if (!compact)
		return hex();
	std::stringstream ss;
	ss << "compact:0x" << std::hex << std::setfill('0') << std::setw(8) << storedHash.compact;
	return ss.str();
}

Hash& Hash::operator<<=(unsigned int shift)
{
	if (shift == 0)
		return *this;
	if (shift >= 0x20 * 8) {
		if (!compact)
			delete[] storedHash.bytes;
		storedHash.compact = 0;
		compact = true;
	}
	if (compact) {
		unsigned char exp = storedHash.compact >> 24;
		while (shift >= 8 && exp < 0x20) {
			exp++;
			shift -= 8;
		}
		uint32_t man = storedHash.compact & 0xFFFFFF;
		while (shift > 0 && (man & 0xC00000) == 0) {
			man <<= 1;
			shift--;
		}
		storedHash.compact = (exp << 24) | (man & 0xFFFFFF);
	}
	if (shift > 0) {
		if (compact)
			expand();
		for (size_t i = shift; i < 0x20; ++i)
			storedHash.bytes[i - shift] = storedHash.bytes[i];
	}
	return *this;
}

Hash& Hash::operator>>=(unsigned int shift)
{
	if (shift == 0)
		return *this;
	if (shift >= 0x20 * 8) {
		if (!compact)
			delete[] storedHash.bytes;
		storedHash.compact = 0;
		compact = true;
	}
	if (compact) {
		unsigned char exp = storedHash.compact >> 24;
		while (shift >= 8 && exp > 0) {
			exp--;
			shift -= 8;
		}
		uint32_t man = storedHash.compact & 0xFFFFFF;
		while (shift > 0 && (man & 0x01) == 0) {
			man >>= 1;
			shift--;
		}
		storedHash.compact = (exp << 24) | (man & 0xFFFFFF);
	}
	if (shift > 0) {
		if (compact)
			expand();
		for (size_t i = 0x1F; i >= shift; --i)
			storedHash.bytes[i] = storedHash.bytes[i - shift];
	}
	return *this;
}

Hash& Hash::operator&=(const Hash& other)
{
	if (compact && storedHash.compact == 0)
		return *this;
	if (other.compact && other.storedHash.compact == 0) {
		if (!compact)
			delete[] storedHash.bytes;
		storedHash.compact = 0;
		compact = true;
		return *this;
	}
	if (!other.compact) {
		expand();
		for (size_t i = 0; i < 0x20; ++i)
			storedHash.bytes[i] &= other.storedHash.bytes[i];
		return *this;
	}
	unsigned char otherExp = other.e();
	uint32_t otherMan = other.m();
	if (compact) {
		unsigned char exp = e();
		if (exp == otherExp)
			storedHash.compact = (exp << 24) | (m() & otherMan);
		else if (exp == otherExp - 1)
			storedHash.compact = (exp << 24) | (m() & (otherMan << 8));
		else if (exp == otherExp - 2)
			storedHash.compact = (exp << 24) | (m() & (otherMan << 16));
		else if (exp == otherExp + 1)
			storedHash.compact = (exp << 24) | (m() & (otherMan >> 8));
		else if (exp == otherExp + 2)
			storedHash.compact = (exp << 24) | (m() & (otherMan >> 16));
		else
			storedHash.compact = 0;
		return *this;
	}
	for (size_t i = 0; i < 0x20; ++i)
		if (i == 0x20 - otherExp)
			storedHash.bytes[i] &= (otherMan >> 16);
		else if (i == 0x20 - otherExp + 1)
			storedHash.bytes[i] &= (otherMan >> 8);
		else if (i == 0x20 - otherExp + 2)
			storedHash.bytes[i] &= (otherMan & 0xFF);
		else
			storedHash.bytes[i] = 0;
	return *this;

}

Hash& Hash::operator|=(const Hash& other)
{
	if (other.compact && other.storedHash.compact == 0)
		return *this;
	if (!other.compact) {
		expand();
		for (size_t i = 0; i < 0x20; ++i)
			storedHash.bytes[i] |= other.storedHash.bytes[i];
		return *this;
	}
	unsigned char otherExp = other.e();
	uint32_t otherMan = other.m();
	if (compact) {
		unsigned char exp = e();
		if (exp == otherExp) {
			storedHash.compact = (exp << 24) | (m() | otherMan);
			return *this;
		}
	}
	expand();
	if (otherExp > 0)
		storedHash.bytes[0x20 - otherExp] |= (otherMan >> 16);
	if (otherExp > 1)
		storedHash.bytes[0x20 - otherExp + 1] |= (otherMan >> 8);
	if (otherExp > 2)
		storedHash.bytes[0x20 - otherExp + 2] |= (otherMan & 0xFF);
	return *this;
}

Hash& Hash::operator^=(const Hash& other)
{
	if (other.compact && other.storedHash.compact == 0)
		return *this;
	if (!other.compact) {
		expand();
		for (size_t i = 0; i < 0x20; ++i)
			storedHash.bytes[i] ^= other.storedHash.bytes[i];
		return *this;
	}
	unsigned char otherExp = other.e();
	uint32_t otherMan = other.m();
	if (compact) {
		unsigned char exp = e();
		if (exp == otherExp) {
			storedHash.compact = (exp << 24) | (m() ^ otherMan);
			return *this;
		}
	}
	expand();
	if (otherExp > 0)
		storedHash.bytes[0x20 - otherExp] ^= (otherMan >> 16);
	if (otherExp > 1)
		storedHash.bytes[0x20 - otherExp + 1] ^= (otherMan >> 8);
	if (otherExp > 2)
		storedHash.bytes[0x20 - otherExp + 2] ^= (otherMan & 0xFF);
	return *this;
}

const Hash Hash::operator~() const
{
	Hash ret = Hash(*this);
	if (!ret.compact) {
		for (size_t i = 0; i < 0x20; ++i)
			ret.storedHash.bytes[i] = ~ret.storedHash.bytes[i];
		return ret;
	}
	uint32_t mantissa = ~(ret.storedHash.compact & 0xFFFFFF);
	unsigned char exponent = ret.storedHash.compact >> 24;
	ret.compact = false;
	ret.storedHash.bytes = new unsigned char[0x20];
	std::memset(ret.storedHash.bytes, 0xFF, 0x20);
	if (exponent > 0)
		ret.storedHash.bytes[0x20 - exponent] = mantissa >> 16;
	if (exponent > 1)
		ret.storedHash.bytes[0x20 - exponent + 1] = mantissa >> 8;
	if (exponent > 2)
		ret.storedHash.bytes[0x20 - exponent + 2] = mantissa & 0xFF;
	return ret;
}

}
}
