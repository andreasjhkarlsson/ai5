#include "StringVariant.h"
#include <wchar.h>
#include <iostream>
#include <Windows.h>

StringVariant::StringVariant(const std::wstring& str): Variant(STRING), str(str)
{

}

StringVariant::~StringVariant(void)
{

}
void StringVariant::print()
{
	std::wcout << "StringVariant: " << str << "\r\n";

}
double StringVariant::toFloating()
{
	return _wtof(str.c_str());
}
__int64 StringVariant::toInteger()
{
	return _wtoi64(str.c_str());
}

bool StringVariant::toBoolean()
{
	return str.length() > 0;
}
const std::wstring& StringVariant::getString()
{
	return str;
}