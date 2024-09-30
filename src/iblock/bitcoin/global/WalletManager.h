#ifndef __IBLOCK_BITCOIN_WALLETMANAGER_H_
#define __IBLOCK_BITCOIN_WALLETMANAGER_H_

#include "iblock/bitcoin/common/BitcoinAddress.h"

namespace iblock
{
namespace bitcoin
{

class Wallet;

class IBLOCK_API WalletManager : public ::omnetpp::cSimpleModule
{
	protected:
		std::vector<Wallet*> wallets = std::vector<Wallet*>();

		virtual void initialize(int stage) override;
		virtual int numInitStages() const override { return 3; }

	public:
		WalletManager() : cSimpleModule() { }

		BitcoinAddress* getRandomBitcoinAddress() const;
};

}
}

#endif
