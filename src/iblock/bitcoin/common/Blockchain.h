#ifndef __IBLOCK_BITCOIN_BLOCKCHAIN_H_
#define __IBLOCK_BITCOIN_BLOCKCHAIN_H_

#include "iblock/bitcoin/objects/Block.h"

namespace iblock
{
namespace bitcoin
{

class IBLOCK_API Blockchain : public ::omnetpp::cOwnedObject
{
	private:
		enum { FL_TKOWNERSHIP = 4 };

	protected:
		int bestChainIndex;
		Block *genesisBlock;
		::omnetpp::cArray *forks;
		::omnetpp::cArray *orphanForks;

		virtual void initialize();

	public:
		Blockchain(const char *name = nullptr, bool tk = false) : cOwnedObject(name) { setTakeOwnership(tk); initialize(); }
		explicit Blockchain(bool tk) : Blockchain(nullptr, tk) {}

		virtual void addGenesisBlock(Block *block) { if (genesisBlock) throw ::omnetpp::cRuntimeError(this, "addGenesisBlock(): Genesis block already set"); genesisBlock = block; forks->addAt(0, block); bestChainIndex = 0; }

		virtual void addBlock(Block *block);

		void setTakeOwnership(bool tk) { setFlag(FL_TKOWNERSHIP, tk); }
		bool getTakeOwnership() const { return flags & FL_TKOWNERSHIP; }
};

}
}

#endif
