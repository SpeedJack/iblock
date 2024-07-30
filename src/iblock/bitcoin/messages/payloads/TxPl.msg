import iblock.bitcoin.messages.payloads.Payload;
import iblock.bitcoin.objects.Transaction;

cplusplus {{
	#include "iblock/bitcoin/objects/Transaction.h"
}}

namespace iblock::bitcoin::payloads;

packet TxPl extends Payload
{
	@customize;
	@toString(.str());
	Transaction *transaction;
}
