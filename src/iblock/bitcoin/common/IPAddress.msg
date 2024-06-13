cplusplus {{
	#include "IPAddress.h"
}}

namespace iblock::bitcoin;

class IPAddress extends cObject
{
	@existingClass;
	@opaque;
	@toString(.str());
	@toValue(.str());
	@fromValue(IPAddress($));
}
