#ifndef __IBLOCK_BITCOIN_NODEMANAGER_H_
#define __IBLOCK_BITCOIN_NODEMANAGER_H_

#include "iblock/bitcoin/bitcoin.h"
#include <cppcoro/generator.hpp>

namespace iblock
{
namespace bitcoin
{

class Wallet;

class IBLOCK_API NodeManager : public ::omnetpp::cSimpleModule
{
	protected:
		std::vector<::omnetpp::cModule*> networkNodes = std::vector<::omnetpp::cModule*>();

		virtual void initialize(int stage) override;
		virtual int numInitStages() const override { return 3; }

	public:
		NodeManager() : cSimpleModule() { }

		cppcoro::generator<::omnetpp::cModule*> nodes() const;
};

}
}

#endif
