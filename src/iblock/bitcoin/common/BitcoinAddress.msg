import iblock.bitcoin.common.Hash;

cplusplus {{
	#include "BitcoinAddress.h"
}}

namespace iblock::bitcoin;

class BitcoinAddress extends cObject
{
	@existingClass;
	@opaque;
	@toString(.str());
	@toValue(.str());
	@fromValue(BitcoinAddress($));
	abstract const Hash hash;
	abstract const string address;
	const cModule* wallet;
	const uint32_t index;
}
