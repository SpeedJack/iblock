cplusplus {{
	#include "Hash.h"
}}

namespace iblock::bitcoin;

class Hash
{
	@existingClass;
	@opaque;
	@toString(.str());
	@toValue(.str());
	@fromValue(Hash($));
}
