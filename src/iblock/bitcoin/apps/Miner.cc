#include "Miner.h"
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
}

void Miner::initialize()
{
	nextBlockMsg = new cMessage("nextBlockMsg");
	hashRate = par("hashRate").doubleValue() * 1000000000;
	//networkHashRate = par("networkHashRate").doubleValue();
	highestTarget = Hash(par("highestTarget").intValue());
	walletAddress = new BitcoinAddress(par("walletAddress").stringValue()); //TODO
	blockchainManager = check_and_cast<BlockchainManager *>(getModuleByPath(par("blockchainStoreModule").stringValue()));
	double ttb = getTimeToBlock();
	EV << "Time to block: " << ttb << endl;
	scheduleAt(simTime() + ttb, nextBlockMsg);
}

double Miner::getTimeToBlock()
{
	Hash target = blockchainManager->getCurrentTargetNBits();
	unsigned int expDiff = (highestTarget.e() - target.e()) << 3;
	double difficulty = std::ldexp(static_cast<double>(highestTarget.m()) / target.m(), expDiff);
	double lambda = hashRate / ((1ULL << 32) * difficulty); // ~ 1/600
	return exponential(1/lambda);
}

void Miner::handleSelfMessage(cMessage *msg)
{
	mineBlock();
	scheduleAt(simTime() + getTimeToBlock(), nextBlockMsg);
}

int64_t Miner::satoshi(double amount)
{
	if (amount < 0)
		return 0;
	return static_cast<int64_t>(std::ceil(amount * 100000000));
}

int64_t Miner::getCurrentBlockReward()
{
	return satoshi(50);
}

void Miner::mineBlock()
{
	const BlockHeader *curHeader = blockchainManager->getCurrentBlockHeader();
	Coinbase *coinbaseTx;
	if (curHeader)
		coinbaseTx = new Coinbase(
			walletAddress,
			getCurrentBlockReward(),
			curHeader->getHeight() + 1
		);
	else
		coinbaseTx = new Coinbase(
			walletAddress,
			getCurrentBlockReward(),
			0
		);

	BlockHeader *header = new BlockHeader();
	header->setVersion(70015); // TODO: get version
	header->setPrevBlockHeader(curHeader);
	header->setMerkleRootHash(Hash(0, 0));
	header->setNBits(blockchainManager->getNextTargetNBits());
	header->setNonce(5678);

	Block *block = new Block(header);
	block->appendTxn(coinbaseTx);

	blockchainManager->addBlock(block);
}

Miner::~Miner()
{
	delete walletAddress;
}

}
}
