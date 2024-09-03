#include "NodeManager.h"
#include "iblock/bitcoin/objects/BlockHeader.h"
#include "iblock/bitcoin/apps/Wallet.h"
#include "omnetpp/ccontextswitcher.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

Define_Module(NodeManager);

void NodeManager::initialize(int stage)
{
	if (stage < 2)
		return;
	//BitcoinAddress* address = new BitcoinAddress("1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa");
	BitcoinAddress* address = wallets.at(0)->getNewAddress();
	CoinbaseInput* txin = new CoinbaseInput();
	TransactionOutput* txout = new TransactionOutput(address, 50 * 100000000);
	Coinbase* coinbaseTx = new Coinbase(txin, txout);

	BlockHeader* header = new BlockHeader();
	header->setVersion(1);
	header->setPrevBlockHeader(nullptr);
	header->setMerkleRootHash(Hash::fromHex("0x4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));
	header->setNBits(Hash(0x1d00ffff));
	header->setNonce(2083236893);

	genesisBlock = new Block(header);
	genesisBlock->appendTxn(coinbaseTx);
}

BitcoinAddress* NodeManager::getRandomBitcoinAddress() const
{
	Enter_Method("getRandomBitcoinAddress()");
	int index = intuniform(0, wallets.size() - 1);
	Wallet* wallet = wallets.at(index);
	return wallet->getNewAddress();
}

}
}
