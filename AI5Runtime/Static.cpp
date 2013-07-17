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
const VariantReference<StringVariant>& StaticString::getVariant()
{
	return strVar;
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

StaticInteger64::StaticInteger64(__int64 value): StaticData(INTEGER64),i64(value)
{

}

__int64 StaticInteger64::getInt()
{
	return i64;
}

StaticInteger32::StaticInteger32(int value): StaticData(INTEGER32),i32(value)
{

}

int StaticInteger32::getInt()
{
	return i32;
}

StaticFloating::StaticFloating(double value): StaticData(FLOATING),floating(value)
{

}

double StaticFloating::getFloating()
{
	return floating;
}