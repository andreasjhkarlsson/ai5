#include "encode.h"
#include <Windows.h>
#include <string>
#include "unicode/format.h"


shared_string Encode::utf8_to_utf16(const char* utf8data,int length)
{
	if(length == 0)
	{
		return create_shared_string(L"");
	}

	return shared_string(new UnicodeString(utf8data,length,"utf-8"));
}

// Encodes UTF-16 little endian into TYPE.
shared_binary Encode::encode(const wchar_t* str,size_t len,TYPE type)
{


	shared_binary res = shared_binary(new std::vector<char>);
	switch(type)
	{
	case CURRENT_CP: // ANSI
		{
		size_t required_size = WideCharToMultiByte(CP_ACP,0,str,len,NULL,0,NULL,NULL);
		res->resize(required_size);
		WideCharToMultiByte(CP_ACP,0,str,len,&(*res)[0],required_size,NULL,NULL);
		}
		break;
	case UTF16_LITTLE_ENDIAN: // UTF-16 little endian.
		{
		size_t utf16_size = len*2;
		res->resize(utf16_size); // TODO: This may not be true for UTF-16.
		memcpy(&(*res)[0],str,utf16_size);
		}
		break;
	case UTF16_BIG_ENDIAN: // UTF-16 big endian.
		{
		size_t utf16_size = len*2;
		res->resize(utf16_size);
		memcpy(&(*res)[0],str,utf16_size);
		swapUtf16Endiness((wchar_t*)&(*res)[0],len);
		}
		break;
	case UTF8: // UTF-8.
		{
		size_t required_size = WideCharToMultiByte(CP_UTF8,0,str,len,NULL,0,NULL,NULL);
		res->resize(required_size);
		WideCharToMultiByte(CP_UTF8,0,str,len,&(*res)[0],required_size,NULL,NULL);
		}
		break;
	}

	return res;
}

// Interpretes binary as TYPE and decodes it as UTF-16 little endian.
shared_string Encode::decode(const char* bin,size_t len,TYPE type)
{
	shared_string result;
	switch(type)
	{
	case CURRENT_CP:
		{
		size_t required_size = MultiByteToWideChar(CP_ACP,0,bin,len,NULL,0);
		std::vector<wchar_t> buffer(required_size);
		size_t converted_size = MultiByteToWideChar(CP_ACP,0,bin,len,&buffer[0],required_size);
		result = shared_string(new UnicodeString(&buffer[0],converted_size));
		}
		break;
	case 2:
		{
			result = shared_string(new UnicodeString((wchar_t*)bin,len/2));
		}
		break;	
	case 3:
		{
			std::vector<char> buffer(len);
			memcpy(&buffer[0],bin,len);
			swapUtf16Endiness((wchar_t*)&buffer[0],buffer.size()/2);
			result = shared_string(new UnicodeString((wchar_t*)&buffer[0],buffer.size()/2));
		}
		break;
	case 4:
		{
		size_t required_size = MultiByteToWideChar(CP_UTF8,0,bin,len,NULL,0);
		std::vector<wchar_t> buffer(required_size);
		MultiByteToWideChar(CP_UTF8,0,bin,len,&buffer[0],required_size);
		result = shared_string(new UnicodeString(&buffer[0],required_size));
		}
		break;
	}	
	return result;
}


void Encode::swapUtf16Endiness(wchar_t* binary,int size)
{

	for(int i=0;i<size;i++)
	{
		char high = (binary[i]>>8)&0xFF;
		char low = binary[i]&0xFF;
		binary[i] = (low<<8)|high;
	}
}