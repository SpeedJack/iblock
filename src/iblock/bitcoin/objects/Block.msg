import iblock.bitcoin.objects.HeavyObject;
import iblock.bitcoin.objects.BlockHeader;
import iblock.bitcoin.objects.Transaction;
import iblock.bitcoin.common.BTCValue;

cplusplus {{
	#include "HeavyObject.h"
	#include "BlockHeader.h"
	#include "Transaction.h"
}}

namespace iblock::bitcoin;

class Block extends HeavyObject
{
	@customize;
	@toString(.str());
	BlockHeader* header @owned;
	abstract const uint32_t height;
	abstract const BTCValue reward @byValue;
	abstract const unsigned long txnCount;
	abstract Transaction* txn[] @opaque;
}
