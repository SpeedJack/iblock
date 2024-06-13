import iblock.bitcoin.messages.payloads.Payload;

namespace iblock::bitcoin::payloads;

packet BlockPl extends Payload
{
	@customize;
	@toString(.str());
}
