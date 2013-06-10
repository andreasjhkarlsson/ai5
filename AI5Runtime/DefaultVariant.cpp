#include "DefaultVariant.h"
#include <string>

DefaultVariant::DefaultVariant(void): Variant(DEFAULT)
{
}


DefaultVariant::~DefaultVariant(void)
{
}


std::wostream& DefaultVariant::format(std::wostream& stream)
{
	stream << L"DefaultVariant: Default";
	return stream;
}
double DefaultVariant::toFloating()
{
	return 0.0;
}
__int64 DefaultVariant::toInteger64()
{
	return 0;
}
int DefaultVariant::toInteger32()
{
	return 0;
}
bool DefaultVariant::toBoolean()
{
	return false;
}
shared_string DefaultVariant::toString()
{
	return shared_string(new std::wstring(L"Default"));
}
bool DefaultVariant::equal(Variant* other)
{
	return other->isDefaultType();
}


DefaultVariant DefaultVariant::Instance;