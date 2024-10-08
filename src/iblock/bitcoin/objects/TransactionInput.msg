import iblock.bitcoin.objects.HeavyOwnedObject;
import iblock.bitcoin.common.Hash;
import iblock.bitcoin.objects.TransactionOutput;
import iblock.bitcoin.common.BTCValue;

cplusplus {{
	#include "HeavyOwnedObject.h"
	#include "TransactionOutput.h"
}}

namespace iblock::bitcoin;

struct Outpoint
{
	Hash txHash;
	uint32_t index;
}

class TransactionInput extends HeavyOwnedObject
{
	@customize;
	@toString(.str());
	abstract TransactionOutput* prevOutput;
	abstract const Outpoint prevOutpoint;
	abstract const BTCValue value @byValue;
	abstract const unsigned int scriptBytes;
	abstract const string signatureScript;
	uint32_t sequence = 0xFFFFFFFF;
}
