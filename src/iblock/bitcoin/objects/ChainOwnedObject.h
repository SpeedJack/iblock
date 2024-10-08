#ifndef __IBLOCK_BITCOIN_CHAINOWNEDOBJECT_H_
#define __IBLOCK_BITCOIN_CHAINOWNEDOBJECT_H_

#include "ChainOwnedObject_m.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API ChainOwnedObject : public ChainOwnedObject_Base
{
	private:
		void copy(const ChainOwnedObject& other) { }
		Hash* hash;

	protected:
		ChainOwnedObject(const char* name=nullptr) : ChainOwnedObject_Base(name) { }
		ChainOwnedObject(const ChainOwnedObject& other) : ChainOwnedObject_Base(other) { copy(other); }
		ChainOwnedObject& operator=(const ChainOwnedObject& other) { if (this == &other) return *this; ChainOwnedObject_Base::operator=(other); copy(other); return *this; }

	public:
		virtual ObjectType getType() const override = 0;
		virtual const Hash& getHash() const override { return *hash; } // TODO: compute

		virtual std::string str() const override
		{
			std::stringstream out;
			out << "type=" << getType() << ", ";
			out << "hash=" << "TODO";
			std::string baseStr = ChainOwnedObject_Base::str();
			if (!baseStr.empty())
				out << "; " << baseStr;
			return out.str();
		}
};

}
}

namespace omnetpp
{

template<> inline iblock::bitcoin::ChainOwnedObject* fromAnyPtr(any_ptr ptr) { return ptr.get<iblock::bitcoin::ChainOwnedObject>(); }

}

#endif
