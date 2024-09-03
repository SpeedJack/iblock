#ifndef __IBLOCK_BITCOIN_COINBASEINPUT_H_
#define __IBLOCK_BITCOIN_COINBASEINPUT_H_

#include "CoinbaseInput_m.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API Block;
class IBLOCK_API Coinbase;

class IBLOCK_API CoinbaseInput : public CoinbaseInput_Base
{
	private:
		void copy(const CoinbaseInput& other) { }

	protected:
		virtual void updateOutpoint() override
		{
			prevOutpoint = new Outpoint();
			prevOutpoint->txHash = Hash(0, 0);
			prevOutpoint->index = 0xFFFFFFFF;
		}

	public:
		CoinbaseInput(const char* name = "CoinbaseInput", const uint32_t height = 0) : CoinbaseInput_Base(name) { setHeight(height); setByteLength(36 + 1 + 4 + 0 + 4); }
		CoinbaseInput(uint32_t height) : CoinbaseInput("CoinbaseInput", height) { }
		CoinbaseInput(const CoinbaseInput& other) : CoinbaseInput_Base(other) { copy(other); }
		CoinbaseInput& operator=(const CoinbaseInput& other) { if (this == &other) return *this; CoinbaseInput_Base::operator=(other); copy(other); return *this; }

		virtual CoinbaseInput* dup() const override { return new CoinbaseInput(*this); }

		virtual const char* getCoinbaseScript() const override { return getSignatureScript(); }
		//virtual void setCoinbaseScript(const char* coinbaseScript) override { setSignatureScript(coinbaseScript); }
		virtual const Coinbase* getTransaction() const;
		virtual int64_t getValue() const override;
};

}
}

namespace omnetpp
{

template<> inline iblock::bitcoin::CoinbaseInput* fromAnyPtr(any_ptr ptr) { return ptr.get<iblock::bitcoin::CoinbaseInput>(); }

}

#endif
