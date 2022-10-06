namespace iblock::bitcoin;

enum Network
{
	Mainnet = 0xf9beb4d9;
	Testnet = 0x0b110907;
	Regtest = 0xfabfb5da;
};

packet Packet
{
	@abstract(true);
	@customize(true);
	Network startString = Network::Mainnet;	
	abstract const string commandName;
	abstract const uint32_t payloadSize;
	abstract const uint32_t computedChecksum;
	uint32_t checksum;
};
