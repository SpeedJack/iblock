#include "TimeRecorder.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Define_Module(TimeRecorder);

void TimeRecorder::initialize()
{
	timerMsg = new cMessage("timer");
	scheduleAt(simTime(), timerMsg);
}

void TimeRecorder::handleMessage(cMessage *msg)
{
	EV_INFO << "Just need to print out something... omnetpp will record the time" << endl;
	scheduleAt(simTime() + SimTime(12*60*60, SIMTIME_S), timerMsg);
}

void TimeRecorder::finish()
{
	EV_INFO << "Just need to print out something... omnetpp will record the time" << endl;
}

TimeRecorder::~TimeRecorder()
{
	cancelAndDelete(timerMsg);
}

}
}
