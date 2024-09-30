cplusplus {{
	#include "BTCValue.h"
}}

namespace iblock::bitcoin;

class BTCValue extends cObject
{
	@existingClass;
	@opaque;
	@toString(.str());
	@fromString(BTCValue($));
	@toValue(.toValue());
	@fromValue(BTCValue($));
}
