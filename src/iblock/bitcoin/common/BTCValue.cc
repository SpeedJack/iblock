#include "BTCValue.h"

using namespace omnetpp;

namespace iblock
{
namespace bitcoin
{

BTCValue::BTCValue(double value, const char* unit)
{
	if (value < 0)
		throw cRuntimeError("BTCValue: value must be non-negative");
	if (UNIT2INT(unit) == UNIT2INT("sat")
			|| UNIT2INT(unit) == UNIT2INT("SAT")
			|| UNIT2INT(unit) == UNIT2INT("TBCb")
			|| UNIT2INT(unit) == UNIT2INT("tbcb"))
		this->value = static_cast<unsigned long long>(value);
	else
		this->value = static_cast<unsigned long long>(convert(value, unit, "sat"));
}

BTCValue::BTCValue(const char* str)
{
	char *endstr;
	double amount = opp_strtod(str, &endstr);
	if (endstr == str)
		throw cRuntimeError("BTCValue: string must contain a numeric value");
	if (amount < 0)
		throw cRuntimeError("BTCValue: value must be non-negative");
	if (*endstr == '\0')
		this->value = static_cast<unsigned long long>(amount);
	else
		this->value = static_cast<unsigned long long>(convert(amount, endstr, "sat"));
}

BTCValue::BTCValue(const cValue& value)
{
	cValue::Type type = value.getType();
	if (type != cValue::Type::DOUBLE && type != cValue::Type::INT)
		throw cRuntimeError("BTCValue: cValue must be of type double or int, not %s", cValue::getTypeName(type));
	const char* unit = value.getUnit();
	if (!unit) {
		if (type == cValue::Type::DOUBLE)
			unit = "BTC";
		else
			unit = "sat";
	}

	if (UNIT2INT(unit) == UNIT2INT("sat")
			|| UNIT2INT(unit) == UNIT2INT("SAT")
			|| UNIT2INT(unit) == UNIT2INT("TBCb")
			|| UNIT2INT(unit) == UNIT2INT("tbcb")) {
		this->value = static_cast<unsigned long long>(value);
		return;
	}
	if (type == cValue::Type::DOUBLE) {
		double val = value.doubleValue();
		if (val < 0)
			throw cRuntimeError("BTCValue: value must be non-negative");
		this->value = static_cast<unsigned long long>(convert(val, unit, "sat"));
	} else {
		intval_t val = value.intValue();
		if (val < 0)
			throw cRuntimeError("BTCValue: value must be non-negative");
		this->value = static_cast<unsigned long long>(convert(val, unit, "sat"));
	}
}

BTCValue::BTCValue(const cPar& par, const char* defaultUnit)
{
	cPar::Type type = par.getType();
	if (type != cPar::Type::DOUBLE && type != cPar::Type::INT)
		throw cRuntimeError("BTCValue: cPar must be of type double or int, not %s", cPar::getTypeName(type));

	const char* parUnit = par.getUnit();
	cProperty* unitProp = par.getProperties()->get("btcUnit");
	if (unitProp)
		parUnit = unitProp->getValue();
	if (!parUnit)
		parUnit = defaultUnit ? defaultUnit
			: (type == cPar::Type::DOUBLE ? "BTC" : "sat");

	if (UNIT2INT(parUnit) == UNIT2INT("sat")
			|| UNIT2INT(parUnit) == UNIT2INT("SAT")
			|| UNIT2INT(parUnit) == UNIT2INT("TBCb")
			|| UNIT2INT(parUnit) == UNIT2INT("tbcb")) {
		this->value = static_cast<unsigned long long>(value);
		return;
	}

	if (type == cPar::Type::DOUBLE) {
		double val = par.doubleValue();
		if (val < 0)
			throw cRuntimeError("BTCValue: value must be non-negative");
		this->value = static_cast<unsigned long long>(convert(val, parUnit, "sat"));
	} else {
		intval_t val = par.intValue();
		if (val < 0)
			throw cRuntimeError("BTCValue: value must be non-negative");
		this->value = static_cast<unsigned long long>(convert(val, parUnit, "sat"));
	}
}

BTCValue::BTCValue(const cProperty& prop, const char* defaultUnit)
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
		throw cRuntimeError("BTCValue: NED property must contain an amount or value field");

	if (amountUnit) {
		if (propUnit)
			throw cRuntimeError("BTCValue: NED property must not contain both a unit attribute and a unit field");
		propUnit = amountUnit;
	}

	char *endstr;
	double amount;
	amount = opp_strtod(amountStr, &endstr);
	if (amountStr == endstr)
		throw cRuntimeError("BTCValue: NED property amount or value field must contain a numeric value");
	if (amount < 0)
		throw cRuntimeError("BTCValue: NED property amount or value field must be non-negative");
	if (*endstr != '\0') {
		if (propUnit)
			throw cRuntimeError("BTCValue: NED property cannot contain both a unit attribute/field and a unit suffix");
		propUnit = endstr;
	}

	if (!propUnit)
		propUnit = defaultUnit ? defaultUnit : "BTC";

	if (UNIT2INT(propUnit) == UNIT2INT("sat")
			|| UNIT2INT(propUnit) == UNIT2INT("SAT")
			|| UNIT2INT(propUnit) == UNIT2INT("TBCb")
			|| UNIT2INT(propUnit) == UNIT2INT("tbcb")) {
		this->value = static_cast<unsigned long long>(value);
		return;
	}

	this->value = static_cast<unsigned long long>(convert(amount, propUnit, "sat"));
}

}
}
