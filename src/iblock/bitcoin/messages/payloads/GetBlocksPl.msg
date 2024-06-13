import iblock.bitcoin.messages.payloads.Payload;

namespace iblock::bitcoin::payloads;

packet GetBlocksPl extends Payload
{
	@customize;
	@toString(.str());
}
