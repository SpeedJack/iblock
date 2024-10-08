#ifndef __IBLOCK_BITCOIN_HEAVYOWNEDOBJECT_H_
#define __IBLOCK_BITCOIN_HEAVYOWNEDOBJECT_H_

#include "iblock/bitcoin/bitcoin.h"
#include "HeavyOwnedObject_m.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API HeavyOwnedObject : public HeavyOwnedObject_Base
{
	private:
		void copy(const HeavyOwnedObject& other) { this->bitLength = other.bitLength; }
		int64_t bitLength = 0;

	protected:
		HeavyOwnedObject(const char* name=nullptr) : HeavyOwnedObject_Base(name) { }
		HeavyOwnedObject(const HeavyOwnedObject& other) : HeavyOwnedObject_Base(other) { copy(other); }
		HeavyOwnedObject& operator=(const HeavyOwnedObject& other) { if (this == &other) return *this; HeavyOwnedObject_Base::operator=(other); copy(other); return *this; }

	public:
		virtual int64_t getBitLength() const override { return bitLength; }
		virtual int32_t getByteLength() const override { return (getBitLength() + 7) >> 3; }
		virtual void setBitLength(int64_t bitLength);
		virtual void setByteLength(int32_t byteLength) { setBitLength(byteLength << 3); }
		virtual void addBitLength(int64_t bitLength);
		virtual void addByteLength(int32_t byteLength) { addBitLength(byteLength << 3); }
		virtual void subtractBitLength(int64_t bitLength) { addBitLength(-bitLength); }
		virtual void subtractByteLength(int32_t byteLength) { subtractBitLength(byteLength << 3); }

		virtual std::string str() const override
		{
			std::stringstream out;
			out << "bitLength=" << getBitLength();
			std::string baseStr = HeavyOwnedObject_Base::str();
			if (!baseStr.empty())
				out << "; " << baseStr;
			return out.str();
		}
};

}
}

namespace omnetpp
{

template<> inline iblock::bitcoin::HeavyOwnedObject* fromAnyPtr(any_ptr ptr) { return ptr.get<iblock::bitcoin::HeavyOwnedObject>(); }

}

#endif
