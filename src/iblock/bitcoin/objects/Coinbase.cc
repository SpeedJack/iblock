#include "Coinbase.h"
#include "iblock/bitcoin/objects/TransactionOutput.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Register_Class(Coinbase)

Coinbase::Coinbase(BitcoinAddress *address, int64_t reward, uint32_t height) : Coinbase()
{
	CoinbaseInput *input = new CoinbaseInput(height);
	this->appendTxIn(input);
	TransactionOutput *output = new TransactionOutput(address, reward);
	this->appendTxOut(output);
}

}
}
