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
	switch (stage) {
	case 0:
		AppBase::initialize(0);
		nextBlockMsg = new cMessage("nextBlockMsg");
		hashRate = par("hashRate").doubleValue() * 1000000000;
		//networkHashRate = par("networkHashRate").doubleValue();
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
	double difficulty = std::ldexp(static_cast<double>(highestTarget.m()) / target.m(), expDiff);
	double lambda = hashRate / ((1ULL << 32) * difficulty); // ~ 1/600
	return exponential(1/lambda);
}

void Miner::handleSelfMessage(cMessage* msg)
{
	mineBlock();
	scheduleAt(simTime() + getTimeToBlock(), nextBlockMsg);
}

void Miner::mineBlock()
{
	const BlockHeader* curHeader = blockchainManager->getCurrentBlockHeader();
	Coinbase* coinbaseTx = new Coinbase(
		walletAddress,
		0,
		curHeader->getHeight() + 1
	);

	BlockHeader* header = new BlockHeader();
	header->setVersion(70015); // TODO: get version
	header->setPrevBlockHeader(curHeader);
	header->setMerkleRootHash(Hash(0, 0));
	header->setNBits(blockchainManager->getNextTargetNBits());
	header->setNonce(5678);

	Block* block = new Block(header);

	uint32_t curBytes = block->getByteLength() + coinbaseTx->getByteLength();
	int64_t fees = 0;
	std::vector<const Transaction*> txns;
	for (const Transaction* txn : mempoolManager->transactions()) {
		uint32_t newBytes = curBytes + txn->getByteLength() + COMPACT_SIZE(txns.size() + 2) - COMPACT_SIZE(txns.size() + 1);
		if (newBytes > 1000*1000*1000)
			break; // or continue?
		txns.push_back(txn);
		fees += txn->getFee();
		curBytes = newBytes;
	}
	coinbaseTx->setOutputValue(getNextSubsidy() + fees);
	txns.insert(txns.begin(), coinbaseTx);
	block->setTxnArraySize(txns.size());
	size_t i = 0;
	for (const Transaction* txn : txns)
		block->setTxn(i++, const_cast<Transaction*>(txn));

	char text[13];
	sprintf(text, "Block mined!");
	bubble(text);
	blockchainManager->addBlock(block);
}

Miner::~Miner()
{
	delete walletAddress;
}

}
}
