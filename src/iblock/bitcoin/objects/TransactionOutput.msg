import iblock.bitcoin.objects.HeavyObject;
import iblock.bitcoin.common.Hash;
import iblock.bitcoin.common.BitcoinAddress;
import iblock.bitcoin.common.BTCValue;

cplusplus {{
	#include "HeavyObject.h"
}}

namespace iblock::bitcoin;

class TransactionOutput extends HeavyObject
{
	@customize;
	@toString(.str());
	BTCValue value @byValue;
	BitcoinAddress* address;
	abstract const unsigned int pkScriptBytes;
	abstract const string pkScript;
}
