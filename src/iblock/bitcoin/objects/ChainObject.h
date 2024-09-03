#ifndef __IBLOCK_BITCOIN_CHAINOBJECT_H_
#define __IBLOCK_BITCOIN_CHAINOBJECT_H_

#include "ChainObject_m.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API ChainObject : public ChainObject_Base
{
	private:
		void copy(const ChainObject& other) { }
		Hash* hash;

	protected:
		ChainObject(const char* name=nullptr) : ChainObject_Base(name) { }
		ChainObject(const ChainObject& other) : ChainObject_Base(other) { copy(other); }
		ChainObject& operator=(const ChainObject& other) { if (this == &other) return *this; ChainObject_Base::operator=(other); copy(other); return *this; }

	public:
		virtual ObjectType getType() const override = 0;
		virtual const Hash& getHash() const override { return *hash; } // TODO: compute

		virtual std::string str() const override
		{
			std::stringstream out;
			out << "type=" << getType() << ", ";
			out << "hash=" << "TODO";
			std::string baseStr = ChainObject_Base::str();
			if (!baseStr.empty())
				out << "; " << baseStr;
			return out.str();
		}
};

}
}

namespace omnetpp
{

template<> inline iblock::bitcoin::ChainObject* fromAnyPtr(any_ptr ptr) { return ptr.get<iblock::bitcoin::ChainObject>(); }

}

#endif
