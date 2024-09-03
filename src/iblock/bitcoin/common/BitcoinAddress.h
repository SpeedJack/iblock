#ifndef __IBLOCK_BITCOIN_BITCOINADDRESS_H_
#define __IBLOCK_BITCOIN_BITCOINADDRESS_H_

#include "iblock/bitcoin/bitcoin.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API Wallet;
class IBLOCK_API Hash;

class IBLOCK_API BitcoinAddress : public ::omnetpp::cObject
{
	private:
		void copy(const BitcoinAddress& other) { this->wallet = other.wallet; this->index = other.index; }

	protected:
		Wallet* wallet;
		uint32_t index;

	public:
		BitcoinAddress() : BitcoinAddress(nullptr, 0) { }
		BitcoinAddress(Wallet* wallet, uint32_t index = 0) : wallet(wallet), index(index) { }
		BitcoinAddress(const char* address);
		BitcoinAddress(const Hash& pubkHash);
		BitcoinAddress(const BitcoinAddress& other) { copy(other); }

		BitcoinAddress& operator=(const BitcoinAddress& other) { if (this == &other) return *this; copy(other); return *this; }
		BitcoinAddress& operator=(const char* address) { return *this = BitcoinAddress(address); }
		BitcoinAddress& operator=(const Hash& pubkHash) { return *this = BitcoinAddress(pubkHash); }

		Hash getPubKHash() const;
		Wallet* getWallet() const { return wallet; }
		uint32_t getIndex() const { return index; }

		bool isWallet(const Wallet* wallet) const;
		bool isSameWallet(const BitcoinAddress& other) const;
		bool isSameWallet(const char* address) const { return isSameWallet(BitcoinAddress(address)); }
		bool isSameWallet(const Hash& pubkHash) const { return isSameWallet(BitcoinAddress(pubkHash)); }

		bool operator==(const BitcoinAddress& other) const { return isSameWallet(other) && index == other.index; }
		bool operator!=(const BitcoinAddress& other) const { return !operator==(other); }

		bool operator==(const char* address) const { return *this == BitcoinAddress(address); }
		bool operator!=(const char* address) const { return !operator==(address); }

		bool operator==(const Hash& pubkHash) const { return *this == BitcoinAddress(pubkHash); }
		bool operator!=(const Hash& pubkHash) const { return !operator==(pubkHash); }

		friend inline bool operator==(const char* a, const BitcoinAddress& b) { return b == BitcoinAddress(a); }
		friend inline bool operator!=(const char* a, const BitcoinAddress& b) { return b != BitcoinAddress(a); }
		friend inline bool operator==(const Hash& a, const BitcoinAddress& b) { return b == BitcoinAddress(a); }
		friend inline bool operator!=(const Hash& a, const BitcoinAddress& b) { return b != BitcoinAddress(a); }

		std::string str() const;
};

}
}

#endif
