#include "BitcoinAddress.h"
#include "Hash.h"
#include "iblock/bitcoin/apps/Wallet.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Register_Class(BitcoinAddress)

BitcoinAddress::BitcoinAddress(const char* address)
{
	wallet = nullptr;
	index = 0;
}

BitcoinAddress::BitcoinAddress(const Hash& pubkHash)
{
	wallet = nullptr;
	index = 0;
}

Hash BitcoinAddress::getPubKHash() const
{
	return Hash(this->wallet->getId(), this->index);
}

std::string BitcoinAddress::str() const
{
	//const unsigned char* hash = getPubKHash().bytes();
	return "TODO";
}

bool BitcoinAddress::isWallet(const Wallet* wallet) const
{
	return this->wallet->getId() == wallet->getId();
}

bool BitcoinAddress::isSameWallet(const BitcoinAddress& other) const
{
	return wallet->getId() == other.wallet->getId();
}

}
}
