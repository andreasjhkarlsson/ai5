#pragma once
#include "types.h"


class Encode
{
public:
	typedef int TYPE;
	static const TYPE CURRENT_CP = 1;
	static const TYPE UTF16_LITTLE_ENDIAN = 2;
	static const TYPE UTF16_BIG_ENDIAN = 3;
	static const TYPE UTF8 = 4;

	// Encodes UTF-16 little endian into TYPE.
	static shared_binary encode(const wchar_t* str,size_t len,TYPE type);
	// Interpretes binary as TYPE and decodes it as UTF-16 little endian.
	static shared_string decode(const char* bin,size_t len,TYPE type);
	// Transcodes UTF-8 into UTF-16 little endian.
	static shared_string utf8_to_utf16(const char* utf8data,int length);
private:
	static void swapUtf16Endiness(wchar_t* binary,int size);
};





