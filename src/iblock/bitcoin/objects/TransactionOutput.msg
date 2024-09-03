import iblock.bitcoin.objects.HeavyObject;
import iblock.bitcoin.common.Hash;
import iblock.bitcoin.common.BitcoinAddress;

cplusplus {{
	#include "HeavyObject.h"
}}

namespace iblock::bitcoin;

class TransactionOutput extends HeavyObject
{
	@customize;
	@toString(.str());
	int64_t value;
	BitcoinAddress* address;
	abstract const unsigned int pkScriptBytes;
	abstract const string pkScript;
}
