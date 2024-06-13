import iblock.bitcoin.objects.ObjectType;
import iblock.bitcoin.common.Hash;

namespace iblock::bitcoin;

class ChainObject extends cOwnedObject
{
	@customize;
	@abstract;
	@toString(.str());
	abstract const ObjectType type;
	abstract const Hash hash;
}
