#ifndef __IBLOCK_BITCOIN_NODEMANAGER_H_
#define __IBLOCK_BITCOIN_NODEMANAGER_H_

#include "iblock/bitcoin/common/BitcoinAddress.h"
#include "iblock/bitcoin/objects/Block.h"

namespace iblock
{
namespace bitcoin
{

class BlockchainManager;
class MempoolManager;
class Wallet;

class IBLOCK_API NodeManager : public ::omnetpp::cSimpleModule
{
	protected:
		std::vector<BlockchainManager*> blockchainManagers = std::vector<BlockchainManager*>();
		std::vector<MempoolManager*> mempoolManagers = std::vector<MempoolManager*>();
		std::vector<Wallet*> wallets = std::vector<Wallet*>();
		Block *genesisBlock;

		virtual void initialize(int stage) override;
		virtual int numInitStages() const override { return 3; }

	public:
		NodeManager() : cSimpleModule() { genesisBlock = nullptr; blockchainManagers.clear(); }

		const std::vector<BlockchainManager*>& getBlockchainManagers() const { Enter_Method_Silent("getBlockchainManagers()"); return blockchainManagers; }
		const std::vector<MempoolManager*>& getMempoolManagers() const { Enter_Method_Silent("getMempoolManagers()"); return mempoolManagers; }
		void registerBlockchainManager(BlockchainManager* node) { Enter_Method_Silent("registerBlockchainManager()"); blockchainManagers.push_back(node); }
		void registerMempoolManager(MempoolManager* node) { Enter_Method_Silent("registerMempoolManager()"); mempoolManagers.push_back(node); }
		Block* getGenesisBlock() const { Enter_Method_Silent("getGenesisBlock()"); return genesisBlock; }

		void registerWallet(Wallet* wallet) { Enter_Method_Silent("registerWallet()"); wallets.push_back(wallet); }
		BitcoinAddress* getRandomBitcoinAddress() const;
};

}
}

#endif
