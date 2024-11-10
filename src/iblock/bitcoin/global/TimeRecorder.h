#ifndef __IBLOCK_BITCOIN_TIMERECORDER_H_
#define __IBLOCK_BITCOIN_TIMERECORDER_H_

#include "iblock/bitcoin/bitcoin.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API TimeRecorder : public ::omnetpp::cSimpleModule
{
	protected:
		::omnetpp::cMessage* timerMsg;

		virtual void initialize() override;
		virtual void handleMessage(::omnetpp::cMessage* msg) override;
		virtual void finish() override;
		~TimeRecorder();
};

}
}

#endif
