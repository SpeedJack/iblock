#include "HashRate.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

inline constexpr HashRate::Unit unitFromString(const char* unit)
{
	switch(UNIT2INT(unit)) {
	case UNIT2INT("YH/s"):
	case UNIT2INT("YH/S"):
	case UNIT2INT("Yh/s"):
	case UNIT2INT("YHps"):
	case UNIT2INT("Yhps"):
	case UNIT2INT("YHPS"):
		return HashRate::YHps;
	case UNIT2INT("ZH/s"):
	case UNIT2INT("ZH/S"):
	case UNIT2INT("Zh/s"):
	case UNIT2INT("ZHps"):
	case UNIT2INT("Zhps"):
	case UNIT2INT("ZHPS"):
		return HashRate::ZHps;
	case UNIT2INT("EH/s"):
	case UNIT2INT("EH/S"):
	case UNIT2INT("Eh/s"):
	case UNIT2INT("EHps"):
	case UNIT2INT("Ehps"):
	case UNIT2INT("EHPS"):
		return HashRate::EHps;
	case UNIT2INT("PH/s"):
	case UNIT2INT("PH/S"):
	case UNIT2INT("Ph/s"):
	case UNIT2INT("PHps"):
	case UNIT2INT("Phps"):
	case UNIT2INT("PHPS"):
		return HashRate::PHps;
	case UNIT2INT("TH/s"):
	case UNIT2INT("TH/S"):
	case UNIT2INT("Th/s"):
	case UNIT2INT("THps"):
	case UNIT2INT("Thps"):
	case UNIT2INT("THPS"):
		return HashRate::THps;
	case UNIT2INT("GH/s"):
	case UNIT2INT("GH/S"):
	case UNIT2INT("Gh/s"):
	case UNIT2INT("GHps"):
	case UNIT2INT("Ghps"):
	case UNIT2INT("GHPS"):
		return HashRate::GHps;
	case UNIT2INT("MH/s"):
	case UNIT2INT("MH/S"):
	case UNIT2INT("Mh/s"):
	case UNIT2INT("MHps"):
	case UNIT2INT("Mhps"):
	case UNIT2INT("MHPS"):
		return HashRate::MHps;
	case UNIT2INT("kH/s"):
	case UNIT2INT("kH/S"):
	case UNIT2INT("kh/s"):
	case UNIT2INT("kHps"):
	case UNIT2INT("khps"):
	case UNIT2INT("kHPS"):
		return HashRate::kHps;
	case UNIT2INT("hH/s"):
	case UNIT2INT("hH/S"):
	case UNIT2INT("hh/s"):
	case UNIT2INT("hHps"):
	case UNIT2INT("hhps"):
	case UNIT2INT("hHPS"):
		return HashRate::hHps;
	case UNIT2INT("daH/s"):
	case UNIT2INT("dah/s"):
	case UNIT2INT("daHps"):
	case UNIT2INT("dahps"):
	case UNIT2INT("daHPS"):
	case UNIT2INT("dH/s"):
	case UNIT2INT("dH/S"):
	case UNIT2INT("dh/s"):
	case UNIT2INT("dHps"):
	case UNIT2INT("dhps"):
	case UNIT2INT("dHPS"):
		return HashRate::daHps;
	case UNIT2INT("H/s"):
	case UNIT2INT("H/S"):
	case UNIT2INT("h/s"):
	case UNIT2INT("Hps"):
	case UNIT2INT("hps"):
	case UNIT2INT("HPS"):
	case UNIT2INT("hash"):
	case UNIT2INT("Hash"):
	case UNIT2INT("HASH"):
		return HashRate::Hps;
	default:
		throw ::omnetpp::cRuntimeError("Unknown unit %s", unit);
	}
}

HashRate::HashRate(const char* str)
{
	char *endstr;
	double amount = opp_strtod(str, &endstr);
	if (endstr == str)
		throw cRuntimeError("HashRate: string must contain a numeric value");
	if (amount < 0)
		throw cRuntimeError("HashRate: value must be non-negative");
	this->value = amount;
	if (*endstr == '\0')
		this->unit = Hps;
	else
		this->unit = unitFromString(endstr);
}

HashRate::HashRate(const cValue& value, Unit unit)
{
	cValue::Type type = value.getType();
	if (type != cValue::Type::DOUBLE && type != cValue::Type::INT)
		throw cRuntimeError("HashRate: cValue must be of type double or int, not %s", cValue::getTypeName(type));
	if (type == cValue::Type::DOUBLE) {
		double val = value.doubleValue();
		if (val < 0)
			throw cRuntimeError("HashRate: value must be non-negative");
		this->value = val;
	} else {
		intval_t val = value.intValue();
		if (val < 0)
			throw cRuntimeError("HashRate: value must be non-negative");
		this->value = static_cast<double>(val);
	}
	this->unit = unit;
}

HashRate::HashRate(const cPar& par, Unit defaultUnit)
{
	cPar::Type type = par.getType();
	if (type != cPar::Type::DOUBLE && type != cPar::Type::INT)
		throw cRuntimeError("HashRate: cPar must be of type double or int, not %s", cPar::getTypeName(type));

	const char* parUnit = par.getUnit();
	cProperty* unitProp = par.getProperties()->get("hrUnit");
	if (unitProp)
		parUnit = unitProp->getValue();
	this->unit = parUnit ? unitFromString(parUnit) : defaultUnit;

	if (type == cPar::Type::DOUBLE) {
		double val = par.doubleValue();
		if (val < 0)
			throw cRuntimeError("HashRate: value must be non-negative");
		this->value = val;
	} else {
		intval_t val = par.intValue();
		if (val < 0)
			throw cRuntimeError("HashRate: value must be non-negative");
		this->value = static_cast<double>(val);
	}
}

HashRate::HashRate(const cProperty& prop, Unit defaultUnit)
{
	const char* propUnit = prop.getValue("unit", 0);
	const char* amountStr = prop.getValue("amount", 0);
	const char* amountUnit = prop.getValue("amount", 1);
	if (!amountStr)
		amountStr = prop.getValue("value", 0);
	if (!amountUnit)
		amountUnit = prop.getValue("value", 1);
	if (!amountStr)
		amountStr = prop.getValue();
	if (!amountUnit)
		amountUnit = prop.getValue(cProperty::DEFAULTKEY, 1);
	if (!amountStr)
		throw cRuntimeError("HashRate: NED property must contain an amount or value field");

	if (amountUnit) {
		if (propUnit)
			throw cRuntimeError("HashRate: NED property must not contain both a unit attribute and a unit field");
		propUnit = amountUnit;
	}

	char *endstr;
	double amount;
	amount = opp_strtod(amountStr, &endstr);
	if (amountStr == endstr)
		throw cRuntimeError("HashRate: NED property amount or value field must contain a numeric value");
	if (amount < 0)
		throw cRuntimeError("HashRate: NED property amount or value field must be non-negative");
	if (*endstr != '\0') {
		if (propUnit)
			throw cRuntimeError("HashRate: NED property cannot contain both a unit attribute/field and a unit suffix");
		propUnit = endstr;
	}

	this->unit = propUnit ? unitFromString(propUnit) : defaultUnit;
	this->value = amount;
}

}
}
