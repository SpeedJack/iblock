import iblock.bitcoin.messages.payloads.Payload;
import iblock.bitcoin.objects.Block;

cplusplus {{
	#include "iblock/bitcoin/objects/Block.h"
}}

namespace iblock::bitcoin::payloads;

packet BlockPl extends Payload
{
	@customize;
	@toString(.str());
	Block *block;
}
