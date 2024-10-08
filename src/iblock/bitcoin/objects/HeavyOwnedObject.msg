namespace iblock::bitcoin;

class HeavyOwnedObject extends cOwnedObject
{
	@customize;
	@abstract;
	@toString(.str());
	abstract const int64_t bitLength;
	abstract const int32_t byteLength;
}
