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

	addUtxoSignal = registerSignal("addUTXO");
	removeUtxoSignal = registerSignal("removeUTXO");
	createAddressSignal = registerSignal("createAddress");
	utxoBTLSignal = registerSignal("utxoBTL");
	miningEarnSignal = registerSignal("miningEarn");

	nextAddressIndex = 0;
	addresses = new cArray("addresses", 32, 64);
	addresses->setTakeOwnership(true);

	mempoolManager = check_and_cast<MempoolManager*>(getModuleByPath(par("mempoolManagerModule").stringValue()));
	mempoolManager->registerWallet(this);

	blockchainManager = check_and_cast<BlockchainManager*>(getModuleByPath(par("blockchainManagerModule").stringValue()));
	blockchainManager->registerWallet(this);

	coinbaseMaturity = par("coinbaseMaturity").intValue();

	utxos.clear();
	onConfirmedBalanceIncreaseCallbacks.clear();
	onUnconfirmedBalanceIncreaseCallbacks.clear();
}

BitcoinAddress* Wallet::getNewAddress()
{
	Enter_Method("getNewAddress()");
	BitcoinAddress* addr = new BitcoinAddress(this, nextAddressIndex++);
	addresses->add(addr);
	emit(createAddressSignal, 1U);
	return addr;
}

void Wallet::addUtxo(std::shared_ptr<const TransactionOutput> utxo)
{
	utxos[utxo] = 0;
	emit(addUtxoSignal, utxo->getValue().sat());
	if (!utxo->isCoinbase() || coinbaseMaturity == 0) {
		if (utxo->isCoinbase())
			emit(miningEarnSignal, utxo->getValue().sat());
		notifyBalanceIncrease(false);
	}
}

void Wallet::confirmUtxo(std::shared_ptr<const TransactionOutput> utxo, bool isGenesis)
{
	if (isGenesis) {
		utxos[utxo] = coinbaseMaturity;
		emit(addUtxoSignal, utxo->getValue().sat());
		notifyBalanceIncrease(false);
		notifyBalanceIncrease(true);
		return;
	}
	auto it = utxos.find(utxo);
	if (it == utxos.end()) {
		utxos[utxo] = 1;
		emit(addUtxoSignal, utxo->getValue().sat());
		if (!utxo->isCoinbase() || coinbaseMaturity == 0)
			notifyBalanceIncrease(false);
		if (!utxo->isCoinbase() || coinbaseMaturity <= 1) {
			if (utxo->isCoinbase())
				emit(miningEarnSignal, utxo->getValue().sat());
			notifyBalanceIncrease(true);
		}
	} else {
		it->second++;
		if (!utxo->isCoinbase() && it->second == 1) {
			notifyBalanceIncrease(true);
		} else if (utxo->isCoinbase() && it->second == coinbaseMaturity) {
			emit(miningEarnSignal, utxo->getValue().sat());
			notifyBalanceIncrease(true);
		}
	}
}

void Wallet::unconfirmUtxo(std::shared_ptr<const TransactionOutput> utxo)
{
	auto it = utxos.find(utxo);
	if (it == utxos.end())
		return;
	if (it->second <= 1) {
		utxos.erase(it);
		emit(removeUtxoSignal, utxo->getValue().sat());
		emit(utxoBTLSignal, it->second);
	} else {
		it->second--;
	}
}

void Wallet::removeUtxo(std::shared_ptr<const TransactionOutput> utxo)
{
	auto it = utxos.find(utxo);
	if (it == utxos.end())
		return;
	emit(removeUtxoSignal, utxo->getValue().sat());
	emit(utxoBTLSignal, it->second);
	utxos.erase(it);
}

std::vector<std::shared_ptr<const TransactionOutput>> Wallet::unspentOutputs(uint32_t minConfirmations) const
{
	Enter_Method("unspentOutputs()");

	std::vector<std::shared_ptr<const TransactionOutput>> result;
	for (const auto& elem : utxos) {
		if (elem.second < minConfirmations)
			continue;
		if (elem.first->isCoinbase() && elem.second < coinbaseMaturity)
			continue;
		std::shared_ptr<const TransactionOutput> utxo = elem.first;
		result.push_back(utxo);
	}
	return result;
}

satoshi_t Wallet::balance(uint32_t minConfirmations) const
{
	Enter_Method("balance()");
	satoshi_t result = 0;
	for (const auto& utxo : utxos) {
		if (utxo.second < minConfirmations)
			continue;
		if (utxo.first->isCoinbase() && utxo.second < coinbaseMaturity)
			continue;
		result += utxo.first->getValue();
	}
	return result;
}

void Wallet::notifyOnBalanceIncrease(std::function<void(void)> callback, bool confirmed)
{
	Enter_Method_Silent("notifyOnBalanceIncrease()");
	if (confirmed)
		onConfirmedBalanceIncreaseCallbacks.push_back(callback);
	else
		onUnconfirmedBalanceIncreaseCallbacks.push_back(callback);
}

void Wallet::notifyBalanceIncrease(bool confirmed)
{
	std::vector<std::function<void(void)>> callbacks(confirmed ? onConfirmedBalanceIncreaseCallbacks : onUnconfirmedBalanceIncreaseCallbacks);
	if (confirmed)
		onConfirmedBalanceIncreaseCallbacks.clear();
	else
		onUnconfirmedBalanceIncreaseCallbacks.clear();
	for (const auto& callback : callbacks)
		callback();
}

}
}
