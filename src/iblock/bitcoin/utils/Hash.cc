#include "Hash.h"
#include <iomanip>

namespace iblock
{
namespace bitcoin
{

Hash::Hash(uint32_t mantissa, unsigned char exponent)
{
	if (exponent > 0x20 || mantissa > 0xFFFFFF ||
			(exponent == 0x20 && mantissa & 0x800000))
		throw ::omnetpp::cRuntimeError("Value too big");

	if (mantissa & 0x800000) {
		mantissa >>= 8;
		exponent++;
	}
	storedHash.compact = ((exponent > 0x20 ? 0x20 : exponent) << 24) | (mantissa & 0xFFFFFF);
	compact = true;
}

Hash::Hash(const unsigned char *bytes)
{
	storedHash.bytes = new unsigned char[0x20];
	std::memcpy(storedHash.bytes, bytes, 0x20);
	compact = false;
}

Hash Hash::fromBytes(const unsigned char *bytes, size_t size)
{
	if (size > 0x20)
		size = 0x20;
	if (size == 0x20)
		return Hash(bytes);
	unsigned char hash[0x20] = { 0 };
	std::memcpy(hash + (0x20 - size), bytes, size);
	return Hash(hash);
}

unsigned char *Hash::bytesCopy() const
{
	unsigned char *bytes = new unsigned char[0x20];
	if (!compact) {
		std::memcpy(bytes, storedHash.bytes, 0x20);
		return bytes;
	}
	uint32_t mantissa = storedHash.compact & 0xFFFFFF;
	unsigned char exponent = storedHash.compact >> 24;
	std::memset(bytes, 0, 0x20);
	if (exponent > 0)
		bytes[0x20 - exponent] = mantissa >> 16;
	if (exponent > 1)
		bytes[0x20 - exponent + 1] = mantissa >> 8;
	if (exponent > 2)
		bytes[0x20 - exponent + 2] = mantissa;
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
		storedHash.bytes[0x20 - exponent + 2] = mantissa;
}

uint32_t Hash::m() const
{
	if (compact)
		return storedHash.compact & 0xFFFFFF;
	unsigned char exponent = e();
	unsigned char mantissa[3] = { 0 };
	size_t i = 0x20 - exponent;
	if (exponent < 0x20 && (storedHash.bytes[i - 1] & 0x80) != 0)
		i--;
	for (; i < 0x20 - exponent + 3 && i < 0x20; ++i)
		mantissa[i - (0x20 - exponent)] = storedHash.bytes[i];
	uint32_t result = (mantissa[0] << 16) | (mantissa[1] << 8) | mantissa[2];
	if (result & 0x800000)
		result >>= 8;
	return result;
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

int32_t Hash::compare(const Hash &other) const
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

	unsigned char *bytes = compact ? bytesCopy() : storedHash.bytes;
	unsigned char *otherBytes = other.compact ? other.bytesCopy() : other.storedHash.bytes;
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
	unsigned char *bytes = compact ? bytesCopy() : storedHash.bytes;
	ss << std::hex << std::setfill('0');
	for (size_t i = 0; i < 0x20; ++i)
		ss << std::setw(2) << static_cast<unsigned int>(bytes[i]);
	if (compact)
		delete[] bytes;
	return ss.str();
}

}
}
