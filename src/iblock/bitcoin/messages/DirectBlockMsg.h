#ifndef __IBLOCK_BITCOIN_DIRECTBLOCKMSG_H_
#define __IBLOCK_BITCOIN_DIRECTBLOCKMSG_H_

#include "DirectBlockMsg_m.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API DirectBlockMsg : public DirectBlockMsg_Base
{
	private:
		void copy(const DirectBlockMsg& other) { this->block = other.block; }

	protected:
		std::shared_ptr<Block> block;

	public:
		DirectBlockMsg() : DirectBlockMsg_Base() { setName("block"); }
		DirectBlockMsg(std::shared_ptr<Block> block) : DirectBlockMsg() { setBlock(block); }
		DirectBlockMsg(const DirectBlockMsg& other) : DirectBlockMsg_Base(other) { copy(other); }
		DirectBlockMsg& operator=(const DirectBlockMsg& other) { if (this == &other) return *this; DirectBlockMsg_Base::operator=(other); copy(other); return *this; }

		virtual DirectBlockMsg* dup() const override { return new DirectBlockMsg(*this); }

		virtual const Block* getBlock() const override { return block.get(); }
		virtual std::shared_ptr<Block> getBlockSharedPtr() const { return block; }
		virtual std::weak_ptr<Block> getBlockWeakPtr() const { return block; }
		virtual void setBlock(Block* block) override { this->block = std::shared_ptr<Block>(block); }
		virtual void setBlock(std::shared_ptr<Block> block) { this->block = block; }
};

}
}

#endif
