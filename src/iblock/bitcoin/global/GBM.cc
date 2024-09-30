#include "GBM.h"
#include "iblock/bitcoin/apps/Wallet.h"
#include "iblock/bitcoin/apps/BlockchainManager.h"
#include "iblock/bitcoin/apps/Miner.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Define_Module(GBM);

Hash GBM::computeInitialNBits() const
{
	cTopology topo;
	topo.extractByProperty("miner");
	hashrate_t networkHashrate = 0;
	for (int i = 0; i < topo.getNumNodes(); ++i) {
		Miner* miner = check_and_cast<Miner*>(topo.getNode(i)->getModule());
		hashrate_t hashrate = miner->par("hashRate");
		networkHashrate += hashrate;
	}
	// double difficulty = networkHashrate.hps() * 600 / (1ULL << 32);
	// double maxTarget = (0xffff * pow(256, 0x1d - 3));
	// double newTarget = maxTarget / difficulty;
	// return Hash(0x1d00ffff);
	double timeAtMinDifficulty = (1ULL << 32) / networkHashrate.hps();
	double ratio = timeAtMinDifficulty / 600.0;
	double newDifficulty = 1.0 / ratio;
	double newTarget = std::ldexp(0xffff, 8 * (0x1d - 3)) / newDifficulty;
	//double newTarget = (0xffff * pow(256, 0x1d - 3)) / newDifficulty;
	return Hash::fromBigDouble(newTarget);
}

void GBM::initialize(int stage)
{
	if (stage < 2)
		return;
	blocks = new cArray("blockchain", 1024, 256);
	blocks->setTakeOwnership(true);

	CoinbaseInput* txin = new CoinbaseInput();

	cTopology topo;
	topo.extractByProperty("wallet");
	std::vector<TransactionOutput*> outputs;
	for (int i = 0; i < topo.getNumNodes(); ++i) {
		cTopology::Node* node = topo.getNode(i);
		Wallet* wallet = check_and_cast<Wallet*>(node->getModule());
		satoshi_t balance = wallet->par("startingBalance");
		if (balance == 0)
			continue;
		BitcoinAddress* address = wallet->getNewAddress();
		TransactionOutput* txout = new TransactionOutput(address, balance);
		outputs.push_back(txout);
	}

	BlockHeader* header = new BlockHeader();
	header->setVersion(1);
	header->setPrevBlockHeader(nullptr);
	header->setMerkleRootHash(Hash::fromHex("0x4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));
	header->setNBits(computeInitialNBits());
	header->setNonce(2083236893);

	coinbaseTx = new Coinbase(txin);
	coinbaseTx->setTxOutArraySize(outputs.size());
	size_t index = 0;
	for (TransactionOutput* txout : outputs)
		coinbaseTx->setTxOut(index++, txout);

	Block* genesisBlock = new Block(header);
	genesisBlock->appendTxn(coinbaseTx);
	blocks->add(genesisBlock);

	topo.clear();
	topo.extractByProperty("blockchainManager");
	for (int i = 0; i < topo.getNumNodes(); ++i) {
		BlockchainManager* bcm = check_and_cast<BlockchainManager*>(topo.getNode(i)->getModule());
		bcm->addGenesisBlock(genesisBlock);
	}
}

void GBM::addBlock(Block* block)
{
	Enter_Method_Silent("addBlock()");
	blocks->add(block);
	totalTx += block->getTxnCount();
	if (totalTx > 1740600 && blocks->size() >= 967)
		throw cTerminationException(E_ENDSIM);
}

}
}
