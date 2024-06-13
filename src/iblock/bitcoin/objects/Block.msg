import iblock.bitcoin.objects.ChainObject;
import iblock.bitcoin.objects.BlockHeader;
import iblock.bitcoin.objects.Transaction;

cplusplus {{
	#include "ChainObject.h"
	#include "BlockHeader.h"
	#include "Transaction.h"
}}

namespace iblock::bitcoin;

class Block extends ChainObject
{
	@customize;
	@toString(.str());
	BlockHeader *header @owned;
	abstract const uint32_t height;
	abstract const unsigned long txnCount;
	Transaction *txn[];
}
