#ifndef __IBLOCK_NODE_COMMON_BLOCKCHAINSTORAGE_H_
#define __IBLOCK_NODE_COMMON_BLOCKCHAINSTORAGE_H_

#include <omnetpp.h>

namespace iblock
{

class BlockchainStorage: public omnetpp::cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(omnetpp::cMessage *msg) override;
};

}

#endif
