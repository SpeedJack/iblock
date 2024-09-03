import iblock.bitcoin.objects.HeavyObject;
import iblock.bitcoin.objects.BlockHeader;
import iblock.bitcoin.objects.Transaction;
import iblock.bitcoin.objects.TransactionOutput;

cplusplus {{
	#include "HeavyObject.h"
	#include "BlockHeader.h"
	#include "Transaction.h"
}}

namespace iblock::bitcoin;

class Block extends HeavyObject
{
	@customize;
	@toString(.str());
	BlockHeader* header @owned;
	abstract const uint32_t height;
	abstract const int64_t reward;
	abstract const unsigned long txnCount;
	Transaction* txn[];
	TransactionOutput* utxo[] @custom @opaque @readonly;
}

cplusplus(Block) {{
	protected:
		std::unordered_map<Wallet*, std::unordered_set<const TransactionOutput*>*> utxos;
	public:
		virtual size_t getUtxoArraySize() const = 0;
		virtual size_t getUtxoCount() const { return getUtxoArraySize(); }
		virtual const TransactionOutput* getUtxo(size_t k) const = 0;
		virtual const std::unordered_set<const TransactionOutput*>* getUtxos(BitcoinAddress* address) const = 0;
		virtual const std::unordered_set<const TransactionOutput*>* getUtxos(Wallet* wallet) const = 0;
		virtual bool hasUtxo(const TransactionOutput* utxo) const = 0;
}}

cplusplus(Block::copy) {{
	utxos = other.utxos;
}}
