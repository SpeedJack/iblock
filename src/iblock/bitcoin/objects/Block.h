#ifndef __IBLOCK_BITCOIN_BLOCK_H_
#define __IBLOCK_BITCOIN_BLOCK_H_

#include "Block_m.h"
#include "Coinbase.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API Block : public Block_Base
{
	private:
		void copy(const Block& other);

	public:
		Block(const char *name = "Block") : Block_Base(name) { }
		Block(BlockHeader *header) : Block() { this->header = header; }
		Block(const Block& other) : Block_Base(other) { copy(other); }
		Block& operator=(const Block& other) { if (this == &other) return *this; Block_Base::operator=(other); copy(other); return *this; }

		virtual Block *dup() const override { return new Block(*this); }

		virtual ObjectType getType() const override { return ObjectType::MSG_BLOCK; }

		virtual unsigned long getTxnCount() const override { return getTxnArraySize(); }

		virtual const Coinbase *getCoinbaseTx() const { return static_cast<const Coinbase *>(getTxn(0)); }

		virtual uint32_t getHeight() const override { return ::omnetpp::check_and_cast<const Coinbase *>(getCoinbaseTx())->getHeight(); }

		virtual std::string str() const override;
};


}
}

#endif
