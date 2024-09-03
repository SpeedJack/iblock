import iblock.bitcoin.objects.ChainObject;
import iblock.bitcoin.common.Hash;

cplusplus {{
	#include "ChainObject.h"
}}

cplusplus(cc) {{
	#include "BlockHeader.h"
}}

namespace iblock::bitcoin;

class BlockHeader extends ChainObject
{
	@customize;
	@toString(.str());
	int32_t version = 70015;
	abstract const Hash prevBlockHeaderHash;
	const BlockHeader* prevBlockHeader;
	Hash merkleRootHash @byValue;
	simtime_t time;
	Hash nBits @byValue;
	uint32_t nonce;
	abstract const uint32_t height;
}
