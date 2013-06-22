#pragma once
#include <Windows.h>
#include <string>

std::wstring utf8_to_utf16(const char* utf8data,int length)
{
	if(length == 0)
	{
		return std::wstring(L"");
	}
	// Allocate temporary buffer (and make sure NULLCHAR fits).
	wchar_t* buff = new wchar_t[length+1];

	// Do the conversion
	int res = MultiByteToWideChar(CP_UTF8,0,utf8data,length,buff,length+1);

	std::wstring str = std::wstring(buff,res);

	// Clean up!
	delete[] buff;

	return str;
}