namespace iblock::bitcoin;

enum Service
{
	NONE = 0x00;
	NODE_NETWORK = 0x01;
	NODE_GETUTXO = 0x02;
	NODE_BLOOM = 0x04;
	NODE_WITNESS = 0x08;
	NODE_XTHIN = 0x10;
	NODE_NETWORK_LIMITED = 0x400;
}
