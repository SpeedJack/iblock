namespace iblock::bitcoin;

class HeavyObject extends cOwnedObject
{
	@customize;
	@abstract;
	@toString(.str());
	abstract const uint64_t bitLength;
	abstract const uint32_t byteLength;
}
