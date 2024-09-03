#include "Transaction.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Register_Class(Transaction);

void Transaction::copy(const Transaction& other)
{
	this->version = other.version;
	unsigned long count = getTxInCount();
	setTxInArraySize(0);
	setTxInArraySize(count);
	for (unsigned long i = 0; i < count; i++)
		appendTxIn(other.txIn[i]->dup());
	count = getTxOutCount();
	setTxOutArraySize(0);
	setTxOutArraySize(count);
	for (unsigned long i = 0; i < count; i++)
		appendTxOut(other.txOut[i]->dup());
}

std::string Transaction::str() const
{
	std::ostringstream out;
	out << "TODO";

	std::string baseStr = Transaction_Base::str();
	if (!baseStr.empty())
		out << "; " << baseStr;
	return out.str();
}

int64_t Transaction::getInputValue() const
{
	int64_t value = 0;
	unsigned long count = getTxInCount();
	for (unsigned long i = 0; i < count; i++) {
		const TransactionInput* txin = getTxIn(i);
		value += txin->getValue();
	}
	return value;
}

int64_t Transaction::getOutputValue() const
{
	int64_t value = 0;
	unsigned long count = getTxOutCount();
	for (unsigned long i = 0; i < count; i++) {
		const TransactionOutput* txout = getTxOut(i);
		value += txout->getValue();
	}
	return value;
}

void Transaction::setTxInArraySize(size_t newSize)
{
	if (newSize > txIn_arraysize) {
		addByteLength(COMPACT_SIZE(newSize) - COMPACT_SIZE(txIn_arraysize));
	} else if (newSize < txIn_arraysize) {
		subtractByteLength(COMPACT_SIZE(txIn_arraysize) - COMPACT_SIZE(newSize));
		for (size_t i = txIn_arraysize - 1; i >= newSize; i--)
			if (txIn[i])
				subtractBitLength(txIn[i]->getBitLength());
	}
	Transaction_Base::setTxInArraySize(newSize);
}

void Transaction::setTxIn(size_t k, TransactionInput* txIn)
{
	Transaction_Base::setTxIn(k, txIn);
	if (txIn)
		addBitLength(txIn->getBitLength());
}

TransactionInput* Transaction::removeTxIn(size_t k)
{
	TransactionInput* txIn = Transaction_Base::removeTxIn(k);
	if (txIn)
		subtractBitLength(txIn->getBitLength());
	return txIn;
}

void Transaction::insertTxIn(size_t k, TransactionInput* txIn)
{
	addByteLength(COMPACT_SIZE(txIn_arraysize + 1) - COMPACT_SIZE(txIn_arraysize));
	Transaction_Base::insertTxIn(k, txIn);
	if (txIn)
		addBitLength(txIn->getBitLength());
}

void Transaction::eraseTxIn(size_t k)
{
	if (k >= txIn_arraysize)
		throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)txIn_arraysize, (unsigned long)k);
	if (this->txIn[k])
		subtractBitLength(this->txIn[k]->getBitLength());
	Transaction_Base::eraseTxIn(k);
	subtractByteLength(COMPACT_SIZE(txIn_arraysize + 1) - COMPACT_SIZE(txIn_arraysize));
}

void Transaction::setTxOutArraySize(size_t newSize)
{
	if (newSize > txOut_arraysize) {
		addByteLength(COMPACT_SIZE(newSize) - COMPACT_SIZE(txOut_arraysize));
	} else if (newSize < txOut_arraysize) {
		subtractByteLength(COMPACT_SIZE(txOut_arraysize) - COMPACT_SIZE(newSize));
		for (size_t i = txOut_arraysize - 1; i >= newSize; i--)
			if (txOut[i])
				subtractBitLength(txOut[i]->getBitLength());
	}
	Transaction_Base::setTxOutArraySize(newSize);
}

void Transaction::setTxOut(size_t k, TransactionOutput* txOut)
{
	Transaction_Base::setTxOut(k, txOut);
	if (txOut)
		addBitLength(txOut->getBitLength());
}

TransactionOutput* Transaction::removeTxOut(size_t k)
{
	TransactionOutput* txOut = Transaction_Base::removeTxOut(k);
	if (txOut)
		subtractBitLength(txOut->getBitLength());
	return txOut;
}

void Transaction::insertTxOut(size_t k, TransactionOutput* txOut)
{
	addByteLength(COMPACT_SIZE(txOut_arraysize + 1) - COMPACT_SIZE(txOut_arraysize));
	Transaction_Base::insertTxOut(k, txOut);
	if (txOut)
		addBitLength(txOut->getBitLength());
}

void Transaction::eraseTxOut(size_t k)
{
	if (k >= txOut_arraysize)
		throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)txOut_arraysize, (unsigned long)k);
	if (this->txOut[k])
		subtractBitLength(this->txOut[k]->getBitLength());
	Transaction_Base::eraseTxOut(k);
	subtractByteLength(COMPACT_SIZE(txOut_arraysize + 1) - COMPACT_SIZE(txOut_arraysize));
}

}
}
