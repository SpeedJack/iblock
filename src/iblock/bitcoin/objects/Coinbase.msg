import iblock.bitcoin.objects.Transaction;

cplusplus {{
	#include "Transaction.h"
}}

namespace iblock::bitcoin;

class Coinbase extends Transaction
{
	@customize;
	@toString(.str());
	abstract const uint32_t height;
	abstract const int64_t reward;
}
