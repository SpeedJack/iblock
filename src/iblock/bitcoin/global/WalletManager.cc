#include "WalletManager.h"
#include "iblock/bitcoin/apps/Wallet.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Define_Module(WalletManager);

void WalletManager::initialize(int stage)
{
	if (stage < 2)
		return;
	cTopology topo;
	topo.extractByProperty("wallet");
	for (int i = 0; i < topo.getNumNodes(); ++i) {
		Wallet* wallet = check_and_cast<Wallet*>(topo.getNode(i)->getModule());
		wallets.push_back(wallet);
	}
}

BitcoinAddress* WalletManager::getRandomBitcoinAddress() const
{
	Enter_Method("getRandomBitcoinAddress()");
	int index = intuniform(0, wallets.size() - 1);
	Wallet* wallet = wallets.at(index);
	return wallet->getNewAddress();
}

}
}
