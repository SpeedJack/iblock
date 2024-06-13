import iblock.bitcoin.messages.payloads.Payload;

namespace iblock::bitcoin::payloads;

packet InvPl extends Payload
{
	@customize;
	@toString(.str());
}
