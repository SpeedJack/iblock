import iblock.bitcoin.objects.ChainObject;
import iblock.bitcoin.objects.TransactionInput;
import iblock.bitcoin.objects.TransactionOutput;

cplusplus {{
	#include "ChainObject.h"
	#include "TransactionInput.h"
	#include "TransactionOutput.h"
}}

namespace iblock::bitcoin;

class Transaction extends ChainObject
{
	@customize;
	@toString(.str());
	int32_t version = 70015;
	abstract const unsigned long txInCount;
	TransactionInput *txIn[] @owned;
	abstract const unsigned long txOutCount;
	TransactionOutput *txOut[] @owned;
	uint32_t lockTime = 0;
}
