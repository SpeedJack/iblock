#include "CoinbaseInput.h"
#include "Coinbase.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Register_Class(CoinbaseInput)

const Coinbase* CoinbaseInput::getTransaction() const
{
	return check_and_cast<const Coinbase*>(getOwner());
}

int64_t CoinbaseInput::getValue() const
{
	const Coinbase* cb = getTransaction();
	if (cb)
		return cb->getReward();
	else
		return 0;
}

}
}
