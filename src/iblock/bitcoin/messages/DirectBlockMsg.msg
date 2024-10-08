import iblock.bitcoin.objects.Block;

cplusplus {{
	#include "iblock/bitcoin/objects/Block.h"
}}

namespace iblock::bitcoin;

packet DirectBlockMsg
{
	@customize;
	@toString(.str());
	abstract Block* block;
}
