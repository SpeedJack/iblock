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
	mainBranch.clear();

	CoinbaseInput* txin = new CoinbaseInput();

	cTopology topo;
	topo.extractByProperty("wallet");
	std::vector<std::shared_ptr<TransactionOutput>> outputs;
	for (int i = 0; i < topo.getNumNodes(); ++i) {
		cTopology::Node* node = topo.getNode(i);
		Wallet* wallet = check_and_cast<Wallet*>(node->getModule());
		satoshi_t balance = wallet->par("startingBalance");
		if (balance == 0)
			continue;
		BitcoinAddress* address = wallet->getNewAddress();
		std::shared_ptr<TransactionOutput> txout = std::make_shared<TransactionOutput>(address, balance);
		outputs.push_back(txout);
	}

	BlockHeader* header = new BlockHeader();
	header->setVersion(1);
	header->setPrevBlock(nullptr);
	header->setMerkleRootHash(Hash::fromHex("0x4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));
	header->setNBits(computeInitialNBits());
	header->setNonce(2083236893);

	std::shared_ptr<Coinbase> coinbaseTx = std::make_shared<Coinbase>(txin);
	coinbaseTx->setTxOutArraySize(outputs.size());
	size_t index = 0;
	for (std::shared_ptr<TransactionOutput> txout : outputs)
		coinbaseTx->setTxOut(index++, txout);

	std::shared_ptr<Block> genesisBlock = std::make_shared<Block>(header);
	genesisBlock->appendTxn(coinbaseTx);

	topo.clear();
	topo.extractByProperty("blockchainManager");
	for (int i = 0; i < topo.getNumNodes(); ++i) {
		BlockchainManager* bcm = check_and_cast<BlockchainManager*>(topo.getNode(i)->getModule());
		bcm->addGenesisBlock(genesisBlock);
		mainBranch[bcm->getId()] = genesisBlock;
	}

	cleanupInterval = par("cleanupInterval").doubleValue();
	cleanupMsg = new cMessage("cleanup");
	scheduleAt(simTime() + cleanupInterval, cleanupMsg);
}

void GBM::handleMessage(cMessage* msg)
{
	if (msg != cleanupMsg) {
		delete msg;
		return;
	}

	uint32_t height = deleteOldBlocks();
	if (height > 0) {
		cTopology topo;
		topo.extractByProperty("blockchainManager");
		for (int i = 0; i < topo.getNumNodes(); ++i) {
			BlockchainManager* bcm = check_and_cast<BlockchainManager*>(topo.getNode(i)->getModule());
			bcm->cleanup(height);
		}
	}
	scheduleAt(simTime() + cleanupInterval, cleanupMsg);
}

std::shared_ptr<const Block> GBM::findForkBlock(std::shared_ptr<const Block> a, std::shared_ptr<const Block> b) const
{
	if (!a || !b)
		throw cRuntimeError("Fork block not resident in memory");
	if (a == b)
		return a;
	if (a->getHeight() > b->getHeight())
		return findForkBlock(a->getPrevBlock(), b);
	if (a->getHeight() < b->getHeight())
		return findForkBlock(a, b->getPrevBlock());
	return findForkBlock(a->getPrevBlock(), b->getPrevBlock());
}

uint32_t GBM::deleteOldBlocks()
{
	std::vector<std::shared_ptr<const Block>> uniqueBranches;
	std::transform(mainBranch.begin(), mainBranch.end(), std::back_inserter(uniqueBranches), [](const auto& kv) {
		return kv.second;
	});
	std::sort(uniqueBranches.begin(), uniqueBranches.end());
	std::vector<std::shared_ptr<const Block>>::iterator it;
	it = std::unique(uniqueBranches.begin(), uniqueBranches.end());
	uniqueBranches.resize(std::distance(uniqueBranches.begin(), it));

	std::shared_ptr<const Block> forkBlock;
	for (auto it = uniqueBranches.begin(); it != uniqueBranches.end(); ++it) {
		if (it == uniqueBranches.begin()) {
			forkBlock = *it;
			continue;
		}
		forkBlock = findForkBlock(forkBlock, *it);
	}
	std::shared_ptr<const Block> parent = forkBlock->getPrevBlock();
	if (!parent)
		return 0;
	uint32_t height = parent->getHeight();
	EV_INFO << "Deleting blocks older than height " << height << endl;
	const_cast<BlockHeader*>(parent->getHeader())->deletePrevBlock();
	return height;
}

GBM::~GBM()
{
	cancelAndDelete(cleanupMsg);
}

}
}
