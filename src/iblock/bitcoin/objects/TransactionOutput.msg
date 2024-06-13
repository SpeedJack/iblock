import iblock.bitcoin.common.Hash;
import iblock.bitcoin.common.BitcoinAddress;

namespace iblock::bitcoin;

class TransactionOutput extends cOwnedObject
{
	@customize;
	@toString(.str());
	int64_t value;
	BitcoinAddress *address;
	abstract const unsigned int pkScriptBytes;
	abstract const string pkScript;
}
