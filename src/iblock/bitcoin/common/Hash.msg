cplusplus {{
	#include "Hash.h"
}}

namespace iblock::bitcoin;

class Hash extends cObject
{
	@existingClass;
	@opaque;
	@toString(.str());
	@toValue(.str());
	@fromValue(Hash($));
}
