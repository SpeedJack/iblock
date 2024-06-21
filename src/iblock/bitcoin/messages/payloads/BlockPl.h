#ifndef __IBLOCK_BITCOIN_PAYLOADS_BLOCK_H_
#define __IBLOCK_BITCOIN_PAYLOADS_BLOCK_H_

#include "BlockPl_m.h"
#include "iblock/bitcoin/messages/MessageKind_m.h"

namespace iblock
{
namespace bitcoin
{
namespace payloads
{

class IBLOCK_API BlockPl : public BlockPl_Base
{
	private:
		void copy(const BlockPl& other) { }
	public:
		BlockPl() : BlockPl_Base() { setKind(MessageKind::BLOCK); setName("block"); setByteLength(0); }
		BlockPl(::iblock::bitcoin::Block *block) : BlockPl() { setBlock(block); }
		BlockPl(const BlockPl& other) : BlockPl_Base(other) { copy(other); }
		BlockPl& operator=(const BlockPl& other) { if (this == &other) return *this; BlockPl_Base::operator=(other); copy(other); return *this; }

		virtual BlockPl *dup() const override { return new BlockPl(*this); }

		virtual unsigned char *getRawBytes() const override;

		virtual std::string str() const override
		{
			std::stringstream out;
			std::string baseStr = BlockPl_Base::str();
			if (!baseStr.empty())
				out << "; " << baseStr;
			return out.str();
		}
};

}
}
}

#endif
