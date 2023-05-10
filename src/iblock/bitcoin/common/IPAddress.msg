cplusplus {{
	#include "IPAddress.h"
}}

namespace iblock::bitcoin;

class IPAddress
{
	@existingClass;
	@opaque;
	@toString(.str());
	@toValue(.str());
	@fromValue(IPAddress($));
}
