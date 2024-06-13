import iblock.bitcoin.objects.TransactionInput;

cplusplus {{
	#include "TransactionInput.h"
}}

namespace iblock::bitcoin;

class CoinbaseInput extends TransactionInput
{
	@customize;
	@toString(.str());
	uint32_t height;
	abstract string coinbaseScript;
}
