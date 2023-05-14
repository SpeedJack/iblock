#ifndef __IBLOCK_BITCOIN_HASH_H_
#define __IBLOCK_BITCOIN_HASH_H_

#include "iblock/iblock.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API Hash
{
	protected:
		union hash_t
		{
			unsigned char *bytes;
			uint32_t compact;
		};

		bool compact;
		hash_t storedHash;

		Hash(const unsigned char *bytes);
		void expand();

	public:
		Hash(uint32_t mantissa, unsigned char exponent);
		Hash(const uint32_t nBits) : Hash(nBits & 0xFFFFFF, nBits >> 24) { }
		Hash() : Hash(0, 0) { }
		Hash(const Hash &other) { compact = other.compact; storedHash = other.storedHash; }

		static Hash fromBytes(const unsigned char *bytes, size_t size = 0x20);

		const unsigned char *bytes() { if (compact) expand(); return storedHash.bytes; }
		unsigned char *bytesPtr() { if (compact) expand(); return storedHash.bytes; }
		unsigned char *bytesCopy() const;
		uint32_t m() const;
		uint32_t mantissa() const { return m(); }
		unsigned char e() const;
		unsigned char exponent() const { return e(); }
		uint32_t nBits() const { return compactNBits(); }
		uint32_t compactNBits() const;
		std::string hex() const;
		bool isCompactPrecise() const;
		int32_t compare(const Hash &other) const;

		Hash& operator=(const Hash &other) { if (this == &other) return *this; compact = other.compact; storedHash = other.storedHash; return *this; }
		Hash& operator=(const uint32_t &nBits) { return *this = Hash(nBits); }

		bool operator==(const Hash &other) const { return compare(other) == 0; }
		bool operator!=(const Hash &other) const { return compare(other) != 0; }
		bool operator<(const Hash &other) const { return compare(other) < 0; }
		bool operator>(const Hash &other) const { return compare(other) > 0; }
		bool operator<=(const Hash &other) const { return compare(other) <= 0; }
		bool operator>=(const Hash &other) const { return compare(other) >= 0; }

		bool operator==(const uint32_t &other) const { return compare(Hash(other)) == 0; }
		bool operator!=(const uint32_t &other) const { return compare(Hash(other)) != 0; }
		bool operator<(const uint32_t &other) const { return compare(Hash(other)) < 0; }
		bool operator>(const uint32_t &other) const { return compare(Hash(other)) > 0; }
		bool operator<=(const uint32_t &other) const { return compare(Hash(other)) <= 0; }
		bool operator>=(const uint32_t &other) const { return compare(Hash(other)) >= 0; }

		friend bool operator==(const uint32_t &a, const Hash &b) { return b.compare(Hash(a)) == 0; }
		friend bool operator!=(const uint32_t &a, const Hash &b) { return b.compare(Hash(a)) != 0; }
		friend bool operator<(const uint32_t &a, const Hash &b) { return b.compare(Hash(a)) > 0; }
		friend bool operator>(const uint32_t &a, const Hash &b) { return b.compare(Hash(a)) < 0; }
		friend bool operator<=(const uint32_t &a, const Hash &b) { return b.compare(Hash(a)) >= 0; }
		friend bool operator>=(const uint32_t &a, const Hash &b) { return b.compare(Hash(a)) <= 0; }
};

}
}

#endif
