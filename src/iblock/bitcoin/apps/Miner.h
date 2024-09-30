#ifndef __IBLOCK_BITCOIN_MINER_H_
#define __IBLOCK_BITCOIN_MINER_H_

#include "iblock/bitcoin/apps/base/AppBase.h"
#include "iblock/bitcoin/common/BitcoinAddress.h"
#include "iblock/bitcoin/common/Hash.h"
#include "iblock/bitcoin/common/HashRate.h"
#include "BlockchainManager.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API Miner : public AppBase
{
	protected:
		hashrate_t hashRate;
		Hash highestTarget;
		BitcoinAddress* walletAddress;
		BlockchainManager* blockchainManager;
		MempoolManager* mempoolManager;
		::omnetpp::cMessage* nextBlockMsg;
		::omnetpp::simsignal_t blockMinedSignalId;
		::omnetpp::simsignal_t processedTransactionsSignalId;

		virtual void initialize(int stage) override;
		virtual int numInitStages() const override { return 4; }
		virtual void handleSelfMessage(::omnetpp::cMessage* msg) override;
		virtual satoshi_t getNextSubsidy() const { return SATOSHIS(50) >> ((blockchainManager->getCurrentHeight() + 1) / 210000); }
		virtual double getTimeToBlock();
		virtual void mineBlock();

	public:
		Miner();
};

}
}

#endif
