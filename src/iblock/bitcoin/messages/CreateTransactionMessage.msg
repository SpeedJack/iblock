import iblock.bitcoin.common.BTCValue;

namespace iblock::bitcoin;

message CreateTransactionMessage
{
	BTCValue amount @byValue;
}
