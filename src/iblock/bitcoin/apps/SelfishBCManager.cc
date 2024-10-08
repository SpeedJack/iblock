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
		selfishChain = std::stack<std::shared_ptr<Block>>();
		maxSelfishChainLength = par("maxSelfishChainLength").intValue();
	}
}

void SelfishBCManager::onBeforeMinedBlockAppend(std::shared_ptr<Block> block)
{
	selfishChainDistance++;
	selfishChain.push(block);
}

void SelfishBCManager::onAfterMinedBlockAppend(std::shared_ptr<Block> block)
{
	if (selfishChain.size() < maxSelfishChainLength)
		return; // we are selfish

	// it's enough...
	std::shared_ptr<Block> forkBlock = selfishChain.top();
	for (unsigned int i = 0; i < selfishChain.size(); i++)
		forkBlock = std::const_pointer_cast<Block>(forkBlock->getPrevBlock());
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

void SelfishBCManager::onNewSideBranch(std::shared_ptr<const Block> branch)
{
	unsigned int distance = computeDistance(branch, mainBranch);
	if (distance < selfishChainDistance) { // Good miners are approaching...
		selfishChainDistance = distance;
		if (selfishChainDistance == 1) // ...we will ruin their day!
			relaySelfishChain(findForkBlock(mainBranch, branch));
	}
}

void SelfishBCManager::onSideBranchAppend(std::shared_ptr<const Block> newBlock)
{
	onNewSideBranch(newBlock);
}

void SelfishBCManager::onNewMainBranch(std::shared_ptr<const Block> oldBranch, std::shared_ptr<const Block> forkBlock)
{
	selfishChainDistance = 0; // Ugh! They won this battle...
	BlockchainManager::onNewMainBranch(oldBranch, forkBlock);
}

void SelfishBCManager::relaySelfishChain(std::shared_ptr<const Block> forkBlock)
{
	while (!selfishChain.empty()) {
		std::shared_ptr<Block> block = selfishChain.top();
		selfishChain.pop();
		BlockchainManager::onAfterMinedBlockAppend(block);
	}
}

}
}
