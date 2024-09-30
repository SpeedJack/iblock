#ifndef __IBLOCK_BITCOIN_STOPSIMULATIONLISTENER_H_
#define __IBLOCK_BITCOIN_STOPSIMULATIONLISTENER_H_

#include "iblock/bitcoin/bitcoin.h"
#include "omnetpp/clistener.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API StopSimulationListener : public ::omnetpp::cListener
{
	protected:
		::omnetpp::simsignal_t blockMinedSignalId;
		::omnetpp::simsignal_t processedTransactionsSignalId;
		unsigned long long processedTransactions = 0;
		unsigned long long blocksMined = 0;

		virtual void receiveSignal(::omnetpp::cComponent* source, ::omnetpp::simsignal_t signalID, ::omnetpp::intval_t value, ::omnetpp::cObject* details) override;
		virtual void receiveSignal(::omnetpp::cComponent* source, ::omnetpp::simsignal_t signalID, ::omnetpp::uintval_t value, ::omnetpp::cObject* details) override;
	public:
		void setBlockMinedSignalId(::omnetpp::simsignal_t blockMinedSignalId) { this->blockMinedSignalId = blockMinedSignalId; }
		void setProcessedTransactionsSignalId(::omnetpp::simsignal_t processedTransactionsSignalId) { this->processedTransactionsSignalId = processedTransactionsSignalId; }
};

}
}

#endif
