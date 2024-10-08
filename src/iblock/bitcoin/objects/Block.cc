#include "Block.h"
#include "BlockHeader.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Register_Class(Block);

void Block::copy(const Block& other)
{
	txn = other.txn;
	for (auto& it : other.utxos) {
		Wallet* wallet = it.first;
		std::unordered_set<std::shared_ptr<const TransactionOutput>>* utxos = it.second;
		std::unordered_set<std::shared_ptr<const TransactionOutput>>* newUtxos = new std::unordered_set<std::shared_ptr<const TransactionOutput>>(); // FIXME: memory leak
		for (std::shared_ptr<const TransactionOutput> utxo : *utxos)
			newUtxos->insert(utxo); // FIXME: memory leak
		this->utxos[wallet] = newUtxos;
	}
}

std::string Block::str() const
{
	std::stringstream out;
	out << "header=[" << header->str() << "], ";
	out << "txn_count=" << getTxnCount();

	std::string baseStr = Block_Base::str();
	if (!baseStr.empty())
		out << "; " << baseStr;
	return out.str();
}

size_t Block::getUtxoArraySize() const
{
	size_t total = 0;
	for (auto& it : utxos)
		if (it.second)
			total += it.second->size();
	return total;
}

// const TransactionOutput* Block::getUtxo(size_t k) const
// {
// 	// size_t total = getUtxoArraySize();
// 	// std::vector<const TransactionOutput*> sorted = std::vector<const TransactionOutput*>(total);
// 	// for (auto& it : utxos)
// 	// 	if (it.second)
// 	// 		for (const TransactionOutput* utxo : *it.second)
// 	// 			sorted.push_back(utxo);
// 	// std::sort(sorted.begin(), sorted.end(), [](const TransactionOutput* a, const TransactionOutput* b) {
// 	// 		int aId = a->getAddress()->getWallet()->getId();
// 	// 		int bId = b->getAddress()->getWallet()->getId();
// 	// 		if (aId != bId)
// 	// 			return aId < bId;
// 	// 		return a->getAddress()->getIndex() < b->getAddress()->getIndex();
// 	// });
// 	// return sorted[k];
// 	for (auto& it : utxos)
// 		if (it.second)
// 			for (std::shared_ptr<const TransactionOutput> utxo : *it.second)
// 				if (k-- == 0)
// 					return utxo;
// 	throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)getUtxoArraySize(), (unsigned long)k);
// }

const std::unordered_set<std::shared_ptr<const TransactionOutput>>* Block::getUtxos(Wallet *wallet) const
{
	auto it = utxos.find(wallet);
	if (it == utxos.end())
		return nullptr;
	return it->second;
}
const std::unordered_set<std::shared_ptr<const TransactionOutput>>* Block::getUtxos(BitcoinAddress *address) const
{
	std::unordered_set<std::shared_ptr<const TransactionOutput>>* found = new std::unordered_set<std::shared_ptr<const TransactionOutput>>();
	const std::unordered_set<std::shared_ptr<const TransactionOutput>>* utxos = getUtxos(address->getWallet());
	if (!utxos)
		return nullptr;

	std::copy_if(utxos->begin(), utxos->end(), std::inserter(*found, found->begin()), [&address](std::shared_ptr<const TransactionOutput> output) {
		return output->getAddress()->getIndex() == address->getIndex();
	});
	return found;
}

// bool Block::hasUtxo(const TransactionOutput *utxo) const
// {
// 	if (!utxo)
// 		return false;
// 	const std::unordered_set<const TransactionOutput*>* utxos = getUtxos(utxo->getAddress()->getWallet());
// 	return utxos->contains(utxo);
// }

void Block::addUtxo(std::shared_ptr<const TransactionOutput> utxo)
{
	if (!utxo)
		return;
	Wallet* wallet = utxo->getAddress()->getWallet();
	auto it = utxos.find(wallet);
	if (it == utxos.end()) {
		std::unordered_set<std::shared_ptr<const TransactionOutput>>* newUtxos = new std::unordered_set<std::shared_ptr<const TransactionOutput>>(); // FIXME: memory leak
		newUtxos->insert(utxo);
		utxos[wallet] = newUtxos;
	} else {
		it->second->insert(utxo); // FIXME: memory leak
	}
}

void Block::removeUtxo(std::shared_ptr<const TransactionOutput> utxo)
{
	if (!utxo)
		return;
	Wallet* wallet = utxo->getAddress()->getWallet();
	auto it = utxos.find(wallet);
	if (it == utxos.end())
		return;
	it->second->erase(utxo);
}

void Block::updateUtxosOnTransactionAdd(std::shared_ptr<Transaction> tx)
{
	size_t txiCount = tx->getTxInCount();
	for (size_t i = 0; i < txiCount; i++)
		removeUtxo(std::const_pointer_cast<const TransactionOutput>(tx->getTxIn(i)->getPrevOutputSharedPtr()));
	size_t txoCount = tx->getTxOutCount();
	for (size_t i = 0; i < txoCount; i++)
		addUtxo(std::const_pointer_cast<const TransactionOutput>(tx->getTxOutSharedPtr(i)));
}

void Block::updateUtxosOnTransactionRemove(std::shared_ptr<Transaction> tx)
{
	size_t txoCount = tx->getTxOutCount();
	for (size_t i = 0; i < txoCount; i++)
		removeUtxo(std::const_pointer_cast<const TransactionOutput>(tx->getTxOutSharedPtr(i)));
	size_t txiCount = tx->getTxInCount();
	for (size_t i = 0; i < txiCount; i++)
		addUtxo(std::const_pointer_cast<const TransactionOutput>(tx->getTxIn(i)->getPrevOutputSharedPtr()));
}

void Block::rebuildUtxos()
{
	eraseUtxos();
	std::shared_ptr<const Block> prevBlock = getPrevBlock();
	if (prevBlock)
		for (auto& it : prevBlock->utxos) {
			Wallet* wallet = it.first;
			std::unordered_set<std::shared_ptr<const TransactionOutput>>* utxos = it.second;
			std::unordered_set<std::shared_ptr<const TransactionOutput>>* newUtxos = new std::unordered_set<std::shared_ptr<const TransactionOutput>>(); // FIXME: memory leak
			for (std::shared_ptr<const TransactionOutput> utxo : *utxos)
				newUtxos->insert(utxo); // FIXME: memory leak
			this->utxos[wallet] = newUtxos;
		}
	for (size_t i = 0; i < getTxnCount(); i++)
		if (txn[i])
			updateUtxosOnTransactionAdd(txn[i]);
}

void Block::eraseUtxos()
{
	for (auto& it : utxos)
		delete it.second;
	utxos.clear();
}

void Block::setTxnArraySize(size_t newSize)
{
	if (newSize > txn.size()) {
		addByteLength(COMPACT_SIZE(newSize) - COMPACT_SIZE(txn.size()));
	} else if (newSize < txn.size()) {
		subtractByteLength(COMPACT_SIZE(txn.size()) - COMPACT_SIZE(newSize));
		for (size_t i = txn.size() - 1; i >= newSize; i--) {
			if (!txn[i])
				continue;
			subtractBitLength(txn[i]->getBitLength());
			updateUtxosOnTransactionRemove(txn[i]);
		}
	}
	txn.resize(newSize);
}

void Block::setTxn(size_t k, std::shared_ptr<Transaction> txn)
{
	if (k >= this->txn.size())
		throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)this->txn.size(), (unsigned long)k);
	if (this->txn[k]) {
		subtractBitLength(this->txn[k]->getBitLength());
		updateUtxosOnTransactionRemove(this->txn[k]);
	}
	this->txn[k] = txn;
	if (txn) {
		addBitLength(txn->getBitLength());
		updateUtxosOnTransactionAdd(txn);
	}
}

void Block::insertTxn(size_t k, std::shared_ptr<Transaction> txn)
{
	addByteLength(COMPACT_SIZE(this->txn.size() + 1) - COMPACT_SIZE(this->txn.size()));
	if (k > this->txn.size())
		throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)this->txn.size(), (unsigned long)k);
	if (k == this->txn.size())
		this->txn.push_back(txn);
	else
		this->txn.insert(this->txn.begin() + k, txn);
	if (txn) {
		addBitLength(txn->getBitLength());
		updateUtxosOnTransactionAdd(txn);
	}
}

void Block::eraseTxn(size_t k)
{
	if (k >= txn.size())
		throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)txn.size(), (unsigned long)k);
	if (this->txn[k]) {
		subtractBitLength(this->txn[k]->getBitLength());
		updateUtxosOnTransactionRemove(this->txn[k]);
	}
	txn.erase(txn.begin() + k);
	subtractByteLength(COMPACT_SIZE(txn.size() + 1) - COMPACT_SIZE(txn.size()));
}

void Block::setHeader(BlockHeader* header)
{
	Block_Base::setHeader(header);
	rebuildUtxos();
}

BlockHeader* Block::removeHeader()
{
	BlockHeader* header = Block_Base::removeHeader();
	if (header)
		rebuildUtxos();
	return header;
}

void Block::replaceHeader(BlockHeader* header)
{
	Block_Base::removeHeader();
	setHeader(header);
}

}
}
