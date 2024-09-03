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

	nodeManager = check_and_cast<NodeManager *>(getModuleByPath(par("nodeManagerModule").stringValue()));
	nodeManager->registerWallet(this);

	mempoolManager = check_and_cast<MempoolManager*>(getModuleByPath(par("mempoolManagerModule").stringValue()));
	mempoolManager->registerWallet(this);

	blockchainManager = check_and_cast<BlockchainManager*>(getModuleByPath(par("blockchainManagerModule").stringValue()));
	blockchainManager->registerWallet(this);

	utxos.clear();
}

BitcoinAddress* Wallet::getNewAddress()
{
	Enter_Method("getNewAddress()");
	return new BitcoinAddress(this, nextAddressIndex++);
}

void Wallet::addUtxo(const TransactionOutput* utxo)
{
	utxos[utxo] = 0;
}

void Wallet::confirmUtxo(const TransactionOutput* utxo)
{
	auto it = utxos.find(utxo);
	if (it == utxos.end())
		utxos[utxo] = 1;
	else
		it->second++;
}

void Wallet::unconfirmUtxo(const TransactionOutput* utxo)
{
	auto it = utxos.find(utxo);
	if (it == utxos.end())
		return;
	if (it->second <= 1)
		utxos.erase(it);
	else
		it->second--;
}

std::vector<const TransactionOutput *> Wallet::unspentOutputs(int64_t minValue, uint32_t minConfirmations) const
{
	Enter_Method("unspentOutputs()");

	std::vector<const TransactionOutput *> result;
	int64_t value = 0;
	for (const auto& utxo : utxos) {
		if (utxo.second < minConfirmations)
			continue;
		result.push_back(utxo.first);
		value += utxo.first->getValue();
		if (value >= minValue)
			break;
	}
	return result;
}

int64_t Wallet::balance(uint32_t minConfirmations) const
{
	int64_t result = 0;
	for (const auto& utxo : utxos)
		if (utxo.second >= minConfirmations)
			result += utxo.first->getValue();
	return result;
}

}
}
