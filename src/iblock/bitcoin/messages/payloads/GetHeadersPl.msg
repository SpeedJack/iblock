import iblock.bitcoin.messages.payloads.Payload;

namespace iblock::bitcoin::payloads;

packet GetHeadersPl extends Payload
{
	@customize;
	@toString(.str());
}
