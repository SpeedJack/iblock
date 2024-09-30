namespace iblock::bitcoin;

class HeavyObject extends cOwnedObject
{
	@customize;
	@abstract;
	@toString(.str());
	abstract const int64_t bitLength;
	abstract const int32_t byteLength;
}
