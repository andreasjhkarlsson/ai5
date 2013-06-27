#include "types.h"
#include <sstream>

std::wstring int2string(int num)
{
	std::wstringstream stream;
	stream << num;
	return stream.str();
}


shared_string create_shared_string(const wchar_t* str,int len)
{
	return shared_string(new std::wstring(str,len));
}


shared_string create_shared_string(const std::wstring& string)
{
	return shared_string(new std::wstring(string));
}