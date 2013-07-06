#include "types.h"
#include <sstream>

UnicodeString int2string(int num)
{
	std::wstringstream stream;
	stream << num;
	return UnicodeString(stream.str().c_str());
}


shared_string create_shared_string(const wchar_t* str,int len)
{
	return shared_string(new UnicodeString(str,len));
}


shared_string create_shared_string(const UnicodeString& string)
{
	return shared_string(new UnicodeString(string));
}

shared_string create_shared_string_from_wstring(const std::wstring& string)
{
	return shared_string(new UnicodeString(string.c_str()));
}