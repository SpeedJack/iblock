namespace iblock::bitcoin;

class Peer extends cOwnedObject
{
	int gateId;
	int32_t version;
	simtime_t timestamp;
	bool acked = false;
};
