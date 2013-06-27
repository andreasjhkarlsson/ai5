#include "Static.h"

StaticData::StaticData(STATIC_TYPE type): type(type)
{
}

STATIC_TYPE StaticData::getType()
{
	return type;
}

StaticString::StaticString(shared_string str): StaticData(STRING), strVar(str)
{

}
StringVariant* StaticString::getVariant()
{
	return &strVar;
}

StaticName::StaticName(shared_string str): StaticData(NAME),name(str)
{

}

shared_string StaticName::getName()
{
	return name;
}

StaticMacro::StaticMacro(shared_string str): StaticData(MACRO),name(str)
{

}

shared_string StaticMacro::getName()
{
	return name;
}

StaticInteger64::StaticInteger64(__int64 value): StaticData(INTEGER64),variant(value)
{

}

Integer64Variant* StaticInteger64::getVariant()
{
	return &variant;
}

StaticInteger32::StaticInteger32(int value): StaticData(INTEGER32),variant(value)
{

}

Integer32Variant* StaticInteger32::getVariant()
{
	return &variant;
}

StaticFloating::StaticFloating(double value): StaticData(FLOATING),variant(value)
{

}

FloatingVariant* StaticFloating::getVariant()
{
	return &variant;
}