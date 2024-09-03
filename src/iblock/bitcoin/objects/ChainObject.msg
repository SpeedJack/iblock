import iblock.bitcoin.objects.HeavyObject;
import iblock.bitcoin.objects.ObjectType;
import iblock.bitcoin.common.Hash;

cplusplus {{
	#include "HeavyObject.h"
}}

namespace iblock::bitcoin;

class ChainObject extends HeavyObject
{
	@customize;
	@abstract;
	@toString(.str());
	abstract const ObjectType type;
	abstract const Hash hash;
}
