import iblock.bitcoin.objects.HeavyObject;
import iblock.bitcoin.common.Hash;
import iblock.bitcoin.objects.TransactionOutput;
import iblock.bitcoin.common.BTCValue;

cplusplus {{
	#include "HeavyObject.h"
	#include "TransactionOutput.h"
}}

namespace iblock::bitcoin;

struct Outpoint
{
	Hash txHash;
	uint32_t index;
}

class TransactionInput extends HeavyObject
{
	@customize;
	@toString(.str());
	TransactionOutput* prevOutput;
	abstract const Outpoint prevOutpoint;
	abstract const BTCValue value @byValue;
	abstract const unsigned int scriptBytes;
	abstract const string signatureScript;
	uint32_t sequence = 0xFFFFFFFF;
}
