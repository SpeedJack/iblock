#include "Wallet.h"
#include "MempoolManager.h"
#include "BlockchainManager.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Define_Module(Wallet)

void Wallet::initialize()
{
	AppBase::initialize();

	nextAddressIndex = 0;
	addresses = new cArray("addresses", 32, 64);
	addresses->setTakeOwnership(true);

	mempoolManager = check_and_cast<MempoolManager*>(getModuleByPath(par("mempoolManagerModule").stringValue()));
	mempoolManager->registerWallet(this);

	blockchainManager = check_and_cast<BlockchainManager*>(getModuleByPath(par("blockchainManagerModule").stringValue()));
	blockchainManager->registerWallet(this);

	utxos.clear();
}

BitcoinAddress* Wallet::getNewAddress()
{
	Enter_Method("getNewAddress()");
	BitcoinAddress* addr = new BitcoinAddress(this, nextAddressIndex++);
	addresses->add(addr);
	return addr;
}

void Wallet::addUtxo(std::shared_ptr<const TransactionOutput> utxo)
{
	utxos[utxo] = 0;
	totalBalance += utxo->getValue().sat();
}

void Wallet::confirmUtxo(std::shared_ptr<const TransactionOutput> utxo)
{
	auto it = utxos.find(utxo);
	if (it == utxos.end()) {
		utxos[utxo] = 1;
		confirmedBalance += utxo->getValue().sat();
		totalBalance += utxo->getValue().sat();
	} else {
		if (it->second == 0)
			confirmedBalance += utxo->getValue().sat();
		it->second++;
	}
}

void Wallet::unconfirmUtxo(std::shared_ptr<const TransactionOutput> utxo)
{
	auto it = utxos.find(utxo);
	if (it == utxos.end())
		return;
	if (it->second <= 1) {
		utxos.erase(it);
		confirmedBalance -= utxo->getValue().sat();
		totalBalance -= utxo->getValue().sat();
	} else {
		it->second--;
	}
}

void Wallet::removeUtxo(std::shared_ptr<const TransactionOutput> utxo)
{
	auto it = utxos.find(utxo);
	if (it == utxos.end())
		return;
	utxos.erase(it);
	confirmedBalance -= utxo->getValue().sat();
	totalBalance -= utxo->getValue().sat();
}

std::vector<std::shared_ptr<const TransactionOutput>> Wallet::unspentOutputs(uint32_t minConfirmations) const
{
	Enter_Method("unspentOutputs()");

	std::vector<std::shared_ptr<const TransactionOutput>> result;
	for (const auto& elem : utxos) {
		if (elem.second < minConfirmations)
			continue;
		std::shared_ptr<const TransactionOutput> utxo = elem.first;
		result.push_back(utxo);
	}
	return result;
}

satoshi_t Wallet::balance(uint32_t minConfirmations) const
{
	Enter_Method("balance()");
	if (minConfirmations == 0)
		return totalBalance;
	if (minConfirmations == 1)
		return confirmedBalance;

	satoshi_t result = 0;
	for (const auto& utxo : utxos) {
		if (utxo.second < minConfirmations)
			continue;
		result += utxo.first->getValue();
	}
	return result;
}

}
}
