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
		void copy(const Hash& other);

		static char hexNibble(const char& c)
		{ return (c >= '0' && c <= '9') ? c - '0' : (c >= 'a' && c <= 'f') ? c - 'a' + 0xA : (c >= 'A' && c <= 'F') ? c - 'A' + 0xA : -1; }

	public:
		Hash(uint32_t mantissa, unsigned char exponent);
		Hash(const uint32_t nBits) : Hash(nBits & 0xFFFFFF, nBits >> 24) { }
		Hash() : Hash(0, 0) { }
		Hash(const Hash &other) { copy(other); }

		~Hash() { if (!compact) delete[] storedHash.bytes; }

		static Hash fromBytes(const unsigned char *bytes, size_t size = 0x20);
		static Hash fromBytes(const char *bytes, size_t size = 0x20) { return fromBytes(reinterpret_cast<const unsigned char *>(bytes)); }
		static Hash fromHex(const std::string hex) { return fromHex(hex.c_str()); }
		static Hash fromHex(const char *hex);

		const unsigned char *bytes() { if (compact) expand(); return storedHash.bytes; }
		unsigned char *bytesPtr() { if (compact) expand(); return storedHash.bytes; }
		unsigned char *bytesCopy() const;
		uint32_t m() const;
		uint32_t mantissa() const { return m(); }
		unsigned char e() const;
		unsigned char exponent() const { return e(); }
		uint32_t nBits() const { return compactNBits(); }
		uint32_t getCompact() const { return compactNBits(); }
		uint32_t compactNBits() const;
		std::string hex() const;
		std::string str() const;
		bool isCompact() const { return compact; }
		bool isCompactPrecise() const;
		void makeCompact();
		bool makeCompactIfPrecise() { if (!compact && isCompactPrecise()) { makeCompact(); return true; } return false; }

		int32_t compare(const Hash& other) const;

		Hash& operator=(const Hash& other) { if (this == &other) return *this; copy(other); return *this; }
		Hash& operator=(const uint32_t& nBits) { return *this = Hash(nBits); }
		Hash& operator=(const unsigned char *bytes) { return *this = Hash::fromBytes(bytes); }
		Hash& operator=(const char *bytes) { return *this = Hash::fromBytes(bytes); }

		bool operator==(const Hash& other) const { return compare(other) == 0; }
		bool operator!=(const Hash& other) const { return compare(other) != 0; }
		bool operator<(const Hash& other) const { return compare(other) < 0; }
		bool operator>(const Hash& other) const { return compare(other) > 0; }
		bool operator<=(const Hash& other) const { return compare(other) <= 0; }
		bool operator>=(const Hash& other) const { return compare(other) >= 0; }

		bool operator==(const uint32_t& other) const { return compare(Hash(other)) == 0; }
		bool operator!=(const uint32_t& other) const { return compare(Hash(other)) != 0; }
		bool operator<(const uint32_t& other) const { return compare(Hash(other)) < 0; }
		bool operator>(const uint32_t& other) const { return compare(Hash(other)) > 0; }
		bool operator<=(const uint32_t& other) const { return compare(Hash(other)) <= 0; }
		bool operator>=(const uint32_t& other) const { return compare(Hash(other)) >= 0; }

		Hash& operator<<=(unsigned int shift);
		Hash& operator>>=(unsigned int shift);

		Hash& operator&=(const Hash& other);
		Hash& operator&=(const uint32_t& other) { return *this &= Hash(other); }
		Hash& operator|=(const Hash& other);
		Hash& operator|=(const uint32_t& other) { return *this |= Hash(other); }
		Hash& operator^=(const Hash& other);
		Hash& operator^=(const uint32_t& other) { return *this ^= Hash(other); }
		const Hash operator~() const;

		friend inline bool operator==(const uint32_t& a, const Hash& b) { return b.compare(Hash(a)) == 0; }
		friend inline bool operator!=(const uint32_t& a, const Hash& b) { return b.compare(Hash(a)) != 0; }
		friend inline bool operator<(const uint32_t& a, const Hash& b) { return b.compare(Hash(a)) > 0; }
		friend inline bool operator>(const uint32_t& a, const Hash& b) { return b.compare(Hash(a)) < 0; }
		friend inline bool operator<=(const uint32_t& a, const Hash& b) { return b.compare(Hash(a)) >= 0; }
		friend inline bool operator>=(const uint32_t& a, const Hash& b) { return b.compare(Hash(a)) <= 0; }

		friend inline const Hash operator<<(const Hash& a, unsigned int shift) { return Hash(a) <<= shift; }
		friend inline const Hash operator>>(const Hash& a, unsigned int shift) { return Hash(a) >>= shift; }

		friend inline const Hash operator&(const Hash& a, const Hash& b) { return Hash(a) &= b; }
		friend inline const Hash operator&(const Hash& a, const uint32_t& b) { return Hash(a) &= Hash(b); }
		friend inline const Hash operator&(const uint32_t& a, const Hash& b) { return Hash(a) &= b; }
		friend inline const Hash operator|(const Hash& a, const Hash& b) { return Hash(a) |= b; }
		friend inline const Hash operator|(const Hash& a, const uint32_t& b) { return Hash(a) |= Hash(b); }
		friend inline const Hash operator|(const uint32_t& a, const Hash& b) { return Hash(a) |= b; }
		friend inline const Hash operator^(const Hash& a, const Hash& b) { return Hash(a) ^= b; }
		friend inline const Hash operator^(const Hash& a, const uint32_t& b) { return Hash(a) ^= Hash(b); }
		friend inline const Hash operator^(const uint32_t& a, const Hash& b) { return Hash(a) ^= b; }
};

}
}

#endif
