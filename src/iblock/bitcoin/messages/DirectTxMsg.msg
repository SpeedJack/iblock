import iblock.bitcoin.objects.Transaction;

cplusplus {{
	#include "iblock/bitcoin/objects/Transaction.h"
}}

namespace iblock::bitcoin;

packet DirectTxMsg
{
	@customize;
	@toString(.str());
	abstract Transaction* tx;
}
