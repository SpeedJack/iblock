import iblock.bitcoin.objects.HeavyOwnedObject;
import iblock.bitcoin.objects.ObjectType;
import iblock.bitcoin.common.Hash;

cplusplus {{
	#include "HeavyOwnedObject.h"
}}

namespace iblock::bitcoin;

class ChainOwnedObject extends HeavyOwnedObject
{
	@customize;
	@abstract;
	@toString(.str());
	abstract const ObjectType type;
	abstract const Hash hash;
}
