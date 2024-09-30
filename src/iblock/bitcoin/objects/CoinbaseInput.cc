#include "CoinbaseInput.h"
#include "Coinbase.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Register_Class(CoinbaseInput)

satoshi_t CoinbaseInput::getValue() const
{
	const Coinbase* cb = check_and_cast_nullable<const Coinbase*>(getTransaction());
	if (cb)
		return cb->getReward();
	else
		return 0;
}

}
}
