#include "StopSimulationListener.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

void StopSimulationListener::receiveSignal(cComponent* source, simsignal_t signalID, intval_t value, cObject* details)
{
	if (signalID == processedTransactionsSignalId)
		processedTransactions++;
	if (blocksMined >= 1456 && processedTransactions >= 2678920)
		throw cTerminationException(E_ENDSIM);
}

void StopSimulationListener::receiveSignal(cComponent* source, simsignal_t signalID, uintval_t value, cObject* details)
{
	if (signalID == blockMinedSignalId)
		blocksMined++;
	if (blocksMined >= 1456 && processedTransactions >= 2678920)
		throw cTerminationException(E_ENDSIM);
}

}
}
