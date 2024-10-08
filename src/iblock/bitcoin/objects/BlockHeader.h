#ifndef __IBLOCK_BITCOIN_BLOCKHEADER_H_
#define __IBLOCK_BITCOIN_BLOCKHEADER_H_

#include <memory>
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
		Hash* hash;
		std::shared_ptr<const Block> prevBlock;

	public:
		BlockHeader(const char* name = "BlockHeader") : BlockHeader_Base(name) { setTime(::omnetpp::simTime()); setByteLength(80); }
		BlockHeader(const BlockHeader& other) : BlockHeader_Base(other) { copy(other); }
		BlockHeader& operator=(const BlockHeader& other) { if (this == &other) return *this; BlockHeader_Base::operator=(other); copy(other); return *this; }

		virtual BlockHeader* dup() const override { return new BlockHeader(*this); }

		virtual ObjectType getType() const override { return ObjectType::MSG_BLOCK; }

		virtual const Hash& getPrevBlockHeaderHash() const override { return getPrevBlockHeader()->getHash(); }
		virtual const Hash& getHash() const override { return *hash; } //TODO computeHash
		virtual uint32_t getHeight() const override;
		virtual const Block* getBlock() const;
		// virtual const Block* getPrevBlock() const { const BlockHeader* h = getPrevBlockHeader(); if (h) return h->getBlock(); else return nullptr; }
		virtual std::shared_ptr<const Block> getPrevBlock() const { return prevBlock; }
		virtual void setPrevBlock(std::shared_ptr<const Block> prevBlock) { this->prevBlock = prevBlock; }
		virtual const BlockHeader* getPrevBlockHeader() const override;
		virtual void setPrevBlockHeader(const BlockHeader* prevBlockHeader) override { throw ::omnetpp::cRuntimeError("Cannot set prevBlockHeader directly. Use setPrevBlock() instead."); }

		virtual void deletePrevBlock() { prevBlock.reset(); }

		virtual std::string str() const override;
};

}
}

namespace omnetpp
{

template<> inline iblock::bitcoin::BlockHeader* fromAnyPtr(any_ptr ptr) { return ptr.get<iblock::bitcoin::BlockHeader>(); }

}

#endif
