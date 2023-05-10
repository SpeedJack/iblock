import iblock.bitcoin.messages.payloads.Payload;

namespace iblock::bitcoin::payloads;

packet PongPl extends Payload
{
	@customize;
	@toString(.str());
	uint64_t nonce = 0;
}
