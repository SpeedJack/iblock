import iblock.bitcoin.messages.payloads.Payload;

namespace iblock::bitcoin::payloads;

packet GetDataPl extends Payload
{
	@customize;
	@toString(.str());
}
