import iblock.bitcoin.messages.payloads.Payload;

namespace iblock::bitcoin::payloads;

packet HeadersPl extends Payload
{
	@customize;
	@toString(.str());
}
