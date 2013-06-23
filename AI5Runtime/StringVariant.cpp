#include "StringVariant.h"
#include <wchar.h>
#include <iostream>
#include <Windows.h>
#include "3rdparty\hsieh_hash.h"

StringVariant::StringVariant(shared_string str): Variant(STRING), str(str)
{

}

StringVariant::StringVariant(const std::wstring& str): Variant(STRING)
{
	this->str = create_shared_string(str);
}

StringVariant::~StringVariant(void)
{

}

std::wostream& StringVariant::format(std::wostream& stream)
{
	stream << "StringVariant: " << *str;
	return stream;

}
double StringVariant::toFloating()
{
	return _wtof(str->c_str());
}
__int64 StringVariant::toInteger64()
{
	return _wtoi64(str->c_str());
}


int StringVariant::toInteger32()
{
	return _wtoi(str->c_str());
}

bool StringVariant::toBoolean()
{
	return str->length() > 0;
}

shared_string StringVariant::toString()
{
	return str;
}

bool StringVariant::equal(Variant* other)
{
	return (getType() == other->getType()) &&
		(wcscmp(str->c_str(),static_cast<StringVariant*>(other)->str->c_str()) == 0);
}


size_t StringVariant::hash()
{
	// Only hash first 32 chars.
	if(str->length() < 32)
		return SuperFastHash((const char*)str->c_str(),str->length()*2);
	return SuperFastHash((const char*)str->c_str(),32*2);
}