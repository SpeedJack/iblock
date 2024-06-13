#ifndef __IBLOCK_BITCOIN_COINBASE_H_
#define __IBLOCK_BITCOIN_COINBASE_H_

#include "Coinbase_m.h"
#include "CoinbaseInput.h"
#include "iblock/bitcoin/common/BitcoinAddress.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API Coinbase : public Coinbase_Base
{
	private:
		void copy(const Coinbase& other) { }

	public:
		Coinbase(const char *name = "Coinbase") : Coinbase_Base(name) { }
		Coinbase(BitcoinAddress *address, int64_t reward, uint32_t height);
		Coinbase(const Coinbase& other) : Coinbase_Base(other) { copy(other); }
		Coinbase& operator=(const Coinbase& other) { if (this == &other) return *this; Coinbase_Base::operator=(other); copy(other); return *this; }

		virtual Coinbase *dup() const override { return new Coinbase(*this); }

		virtual uint32_t getHeight() const override { return ::omnetpp::check_and_cast<const CoinbaseInput *>(getTxIn(0))->getHeight(); }
};

}
}

#endif
