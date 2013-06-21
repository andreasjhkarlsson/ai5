#include "string_functions.h"
#include "..\AI5Runtime\StringVariant.h"
#include "..\AI5Runtime\Integer32Variant.h"
#include <string>
#include <wchar.h>

namespace AI5StandardLibrary
{


Variant* stringupper(Variant** args,int argsSize)
{
	shared_string arg = args[0]->toString();
	size_t buff_size = arg->length()+1;
	wchar_t* buffer = new wchar_t[buff_size];
	wcscpy_s(buffer,buff_size,arg->c_str());
	
	_wcsupr_s(buffer,buff_size);
	shared_string ret = shared_string(new std::wstring(buffer));
	delete buffer;
	return new StringVariant(ret);
}


Variant* stringlower(Variant** args,int argsSize)
{
	shared_string arg = args[0]->toString();
	size_t buff_size = arg->length()+1;
	wchar_t* buffer = new wchar_t[buff_size];
	wcscpy_s(buffer,buff_size,arg->c_str());
	_wcslwr_s(buffer,buff_size);
	shared_string ret = shared_string(new std::wstring(buffer));
	delete buffer;
	return new StringVariant(ret);
}

Variant* stringlen(Variant** args,int argsSize)
{
	return new Integer32Variant(args[0]->toString()->length());
}



}