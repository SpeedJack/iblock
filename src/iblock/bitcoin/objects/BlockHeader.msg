import iblock.bitcoin.objects.ChainOwnedObject;
import iblock.bitcoin.common.Hash;

cplusplus {{
	#include "ChainOwnedObject.h"
}}

cplusplus(cc) {{
	#include "BlockHeader.h"
}}

namespace iblock::bitcoin;

class BlockHeader extends ChainOwnedObject
{
	@customize;
	@toString(.str());
	int32_t version = 70015;
	abstract const Hash prevBlockHeaderHash;
	abstract const BlockHeader* prevBlockHeader;
	Hash merkleRootHash @byValue;
	simtime_t time;
	Hash nBits @byValue;
	uint32_t nonce;
	abstract const uint32_t height;
}
