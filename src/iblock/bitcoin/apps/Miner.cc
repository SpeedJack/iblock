#include "Miner.h"
#include "Wallet.h"
#include "iblock/bitcoin/objects/Block.h"

using namespace omnetpp;

Define_Module(iblock::bitcoin::Miner);

namespace iblock
{
namespace bitcoin
{

Miner::Miner() : AppBase()
{
	walletAddress = nullptr;
	blockchainManager = nullptr;
	mempoolManager = nullptr;
}

void Miner::initialize(int stage)
{
	// StopSimulationListener* listener;
	switch (stage) {
	case 0:
		AppBase::initialize(0);
		blockMinedSignalId = registerSignal("blockMined");
		processedTransactionsSignalId = registerSignal("processedTransactions");
		// listener = new StopSimulationListener();
		// subscribe("blockMined", listener);
		// subscribe("processedTransactions", listener);
		nextBlockMsg = new cMessage("nextBlockMsg");
		hashRate = par("hashRate");
		highestTarget = Hash(par("highestTarget").intValue());
		blockchainManager = check_and_cast<BlockchainManager*>(getModuleByPath(par("blockchainStoreModule").stringValue()));
		mempoolManager = check_and_cast<MempoolManager*>(getModuleByPath(par("mempoolManagerModule").stringValue()));
		break;
	case 1:
		AppBase::initialize(1);
		break;
	case 2:
		AppBase::initialize(2);
		break;
	case 3:
		AppBase::initialize(3);
		Wallet* wallet = check_and_cast<Wallet*>(getModuleByPath(par("walletModule").stringValue()));
		walletAddress = wallet->getNewAddress();
		double ttb = getTimeToBlock();
		EV << "Time to block: " << ttb << endl;
		scheduleAt(simTime() + ttb, nextBlockMsg);
	}
}

double Miner::getTimeToBlock()
{
	Hash target = blockchainManager->getCurrentTargetNBits();
	unsigned int expDiff = (highestTarget.e() - target.e()) << 3;
	double difficulty = std::ldexp(highestTarget.m() / static_cast<double>(target.m()), expDiff);
	double meanTime = difficulty * (1ULL << 32) / hashRate;
	return exponential(meanTime);
}

void Miner::handleSelfMessage(cMessage* msg)
{
	mineBlock();
	scheduleAt(simTime() + getTimeToBlock(), nextBlockMsg);
}

void Miner::mineBlock()
{
	std::shared_ptr<const Block> curBlock = blockchainManager->getCurrentBlock();
	std::shared_ptr<Coinbase> coinbaseTx = std::make_shared<Coinbase>(
		walletAddress,
		0,
		curBlock->getHeight() + 1
	);

	BlockHeader* header = new BlockHeader();
	header->setVersion(70015);
	header->setPrevBlock(curBlock);
	header->setMerkleRootHash(Hash(0, 0));
	header->setNBits(blockchainManager->getNextTargetNBits());
	header->setNonce(5678);

	std::shared_ptr<Block> block = std::make_shared<Block>(header);

	int32_t curBytes = block->getByteLength() + coinbaseTx->getByteLength();
	satoshi_t fees = 0;
	std::vector<std::shared_ptr<const Transaction>> txns;
	for (std::shared_ptr<const Transaction> txn : mempoolManager->transactions()) {
		int32_t newBytes = curBytes + txn->getByteLength() + COMPACT_SIZE(txns.size() + 2) - COMPACT_SIZE(txns.size() + 1);
		if (newBytes > 1000*1000)
			continue;
		txns.push_back(txn);
		fees += txn->getFee();
		curBytes = newBytes;
	}
	coinbaseTx->getTxOutForUpdate(0)->setValue(getNextSubsidy() + fees);
	txns.insert(txns.begin(), coinbaseTx);
	block->setTxnArraySize(txns.size());
	size_t i = 0;
	for (std::shared_ptr<const Transaction> txn : txns)
		block->setTxn(i++, std::const_pointer_cast<Transaction>(txn));

	size_t mempoolBefore = mempoolManager->transactionsCount();
	mempoolManager->addTransaction(coinbaseTx);
	blockchainManager->addBlock(block);
	emit(blockMinedSignalId, 1);
	//emit(processedTransactionsSignalId, block->getTxnCount());
	EV_INFO << "Mined block (height=" << block->getHeight() << "; txn=" << block->getTxnCount() << "; bytes=" << block->getByteLength() << ")" << endl;
	EV_INFO << "Transactions in mempool: " << "before=" << mempoolBefore << "; after=" << mempoolManager->transactionsCount() << "; simtime=" << simTime() << endl;
}

}
}
