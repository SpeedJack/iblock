#ifndef __IBLOCK_BITCOIN_HEAVYOBJECT_H_
#define __IBLOCK_BITCOIN_HEAVYOBJECT_H_

#include "HeavyObject_m.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API HeavyObject : public HeavyObject_Base
{
	private:
		void copy(const HeavyObject& other) { this->bitLength = other.bitLength; }
		uint64_t bitLength = 0;

	protected:
		HeavyObject(const char* name=nullptr) : HeavyObject_Base(name) { }
		HeavyObject(const HeavyObject& other) : HeavyObject_Base(other) { copy(other); }
		HeavyObject& operator=(const HeavyObject& other) { if (this == &other) return *this; HeavyObject_Base::operator=(other); copy(other); return *this; }

		virtual void setBitLength(uint64_t bitLength) { this->bitLength = bitLength; }
		virtual void setByteLength(uint32_t byteLength) { setBitLength(byteLength << 3); }
		virtual void addBitLength(uint64_t bitLength) { setBitLength(this->bitLength + bitLength); }
		virtual void addByteLength(uint32_t byteLength) { addBitLength(byteLength << 3); }
		virtual void subtractBitLength(uint64_t bitLength) { setBitLength(this->bitLength - bitLength); }
		virtual void subtractByteLength(uint32_t byteLength) { subtractBitLength(byteLength << 3); }

	public:
		virtual uint64_t getBitLength() const override { return bitLength; }
		virtual uint32_t getByteLength() const override { return (getBitLength() + 7) >> 3; }

		virtual std::string str() const override
		{
			std::stringstream out;
			out << "bitLength=" << getBitLength();
			std::string baseStr = HeavyObject_Base::str();
			if (!baseStr.empty())
				out << "; " << baseStr;
			return out.str();
		}
};

}
}

namespace omnetpp
{

template<> inline iblock::bitcoin::HeavyObject* fromAnyPtr(any_ptr ptr) { return ptr.get<iblock::bitcoin::HeavyObject>(); }

}

#endif
