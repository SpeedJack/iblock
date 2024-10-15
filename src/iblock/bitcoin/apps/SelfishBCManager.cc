#include "SelfishBCManager.h"
#include "Wallet.h"
#include <stack>

using namespace omnetpp;
using namespace iblock::bitcoin::payloads;

namespace iblock
{
namespace bitcoin
{

Define_Module(SelfishBCManager)

void SelfishBCManager::initialize(int stage)
{
	BlockchainManager::initialize(stage);
	if (stage == 0) {
		selfishChain = std::deque<std::shared_ptr<Block>>();
		maxSelfishChainLength = par("maxSelfishChainLength").intValue();

		selfishChainLengthSignal = registerSignal("selfishChainLength");
		selfishChainDistanceSignal = registerSignal("selfishChainDistance");
		attackResultSignal = registerSignal("attackResult");
	}
}

void SelfishBCManager::onBeforeMinedBlockAppend(std::shared_ptr<Block> block)
{
	BlockchainManager::onBeforeMinedBlockAppend(block);
	selfishChainDistance++;
	selfishChain.push_back(block);
	emit(selfishChainDistanceSignal, selfishChainDistance);
	emit(selfishChainLengthSignal, selfishChain.size());
}

void SelfishBCManager::onAfterMinedBlockAppend(std::shared_ptr<Block> block)
{
	if (selfishChain.size() < maxSelfishChainLength)
		return; // we are selfish

	// it's enough...
	std::shared_ptr<const Block> forkBlock = selfishChain.front()->getPrevBlock();
	relaySelfishChain(forkBlock);
}

unsigned int SelfishBCManager::computeDistance(std::shared_ptr<const Block> first, std::shared_ptr<const Block> second)
{
	uint32_t firstHeight = first->getHeight();
	uint32_t secondHeight = second->getHeight();
	if (firstHeight > secondHeight)
		return firstHeight - secondHeight;
	else
		return secondHeight - firstHeight;
}

void SelfishBCManager::relaySelfishChainIfNeeded(std::shared_ptr<const Block> receivedBlock)
{
	unsigned int distance = computeDistance(receivedBlock, mainBranch);
	if (distance < selfishChainDistance) { // Good miners are approaching...
		selfishChainDistance = distance;
		emit(selfishChainDistanceSignal, selfishChainDistance);
		if (selfishChainDistance == 1) // ...we will ruin their day!
			relaySelfishChain(findForkBlock(mainBranch, receivedBlock));
	}
}

void SelfishBCManager::onNewSideBranch(std::shared_ptr<const Block> branch)
{
	BlockchainManager::onNewSideBranch(branch);
	relaySelfishChainIfNeeded(branch);
}

void SelfishBCManager::onSideBranchAppend(std::shared_ptr<const Block> newBlock)
{
	BlockchainManager::onSideBranchAppend(newBlock);
	relaySelfishChainIfNeeded(newBlock);
}

void SelfishBCManager::onNewMainBranch(std::shared_ptr<const Block> oldBranch, std::shared_ptr<const Block> forkBlock)
{
	if (selfishChainDistance > 0)
		emit(attackResultSignal, false);
	selfishChainDistance = 0; // Ugh! They won this battle...
	selfishChain.clear();
	emit(selfishChainDistanceSignal, 0U);
	emit(selfishChainLengthSignal, 0U);
	BlockchainManager::onNewMainBranch(oldBranch, forkBlock);
}

void SelfishBCManager::relaySelfishChain(std::shared_ptr<const Block> forkBlock)
{
	emit(attackResultSignal, true);
	while (!selfishChain.empty()) {
		std::shared_ptr<Block> block = selfishChain.front();
		selfishChain.pop_front();
		BlockchainManager::onAfterMinedBlockAppend(block);
	}
	selfishChainDistance = 0;
	emit(selfishChainLengthSignal, 0U);
}

}
}
