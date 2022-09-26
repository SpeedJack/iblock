namespace iblock::bitcoin;

class BlockHeader extends cOwnedObject
{
	int32_t nVersion = 4;
	char hashPrevBlock[32];
	Block *prevBlock;
	char hashMerleRoot[32];
	uint32_t nTime;
	uint32_t nBits;
	uint32_t nNonce;
}

class Block extends cOwnedObject
{
	BlockHeader *header @owned;
	unsigned int txnCount;
	// TODO: add array of transactions
}
