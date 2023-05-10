import iblock.bitcoin.messages.payloads.Payload;

namespace iblock::bitcoin::payloads;

packet VerackPl extends Payload
{
	@customize;
	@toString(.str());
}
