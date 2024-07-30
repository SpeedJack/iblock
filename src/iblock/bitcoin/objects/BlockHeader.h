#ifndef __IBLOCK_BITCOIN_BLOCKHEADER_H_
#define __IBLOCK_BITCOIN_BLOCKHEADER_H_

#include "BlockHeader_m.h"

namespace iblock
{
namespace bitcoin
{

class Block;

class IBLOCK_API BlockHeader : public BlockHeader_Base
{
	private:
		void copy(const BlockHeader& other);
		Hash *hash;

	public:
		BlockHeader(const char *name = "BlockHeader") : BlockHeader_Base(name) { setTime(::omnetpp::simTime()); }
		BlockHeader(const BlockHeader& other) : BlockHeader_Base(other) { copy(other); }
		BlockHeader& operator=(const BlockHeader& other) { if (this == &other) return *this; BlockHeader_Base::operator=(other); copy(other); return *this; }

		virtual BlockHeader *dup() const override { return new BlockHeader(*this); }

		virtual const Hash& getPrevBlockHeaderHash() const override { return getPrevBlockHeader()->getHash(); }
		virtual const Hash& getHash() const override { return *hash; } //TODO computeHash
		virtual uint32_t getHeight() const override;
		virtual const Block *getBlock() const;
		virtual const Block *getPrevBlock() const { return getPrevBlockHeader()->getBlock(); }

		virtual std::string str() const override;
};

}
}

namespace omnetpp
{

template<> inline iblock::bitcoin::BlockHeader *fromAnyPtr(any_ptr ptr) { return check_and_cast<iblock::bitcoin::BlockHeader *>(ptr.get<cObject>()); }

}

#endif
