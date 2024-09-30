import iblock.bitcoin.objects.Transaction;
import iblock.bitcoin.common.BTCValue;

cplusplus {{
	#include "Transaction.h"
}}

namespace iblock::bitcoin;

class Coinbase extends Transaction
{
	@customize;
	@toString(.str());
	abstract const uint32_t height;
	abstract const BTCValue reward @byValue;
}
