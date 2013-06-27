#include "StringFunctions.h"
#include "..\AI5Runtime\StringVariant.h"
#include "..\AI5Runtime\Integer32Variant.h"
#include "..\AI5Runtime\BooleanVariant.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\CallInfo.h"
#include <string>
#include <wchar.h>
#include <functional>
#include <memory>
using namespace std::placeholders;

StringFunctions::StringFunctions(void)
{
}


StringFunctions::~StringFunctions(void)
{
}


Variant* StringFunctions::stringUpper(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	shared_string arg = callInfo->getStringArg(0);
	size_t buff_size = arg->length()+1;
	wchar_t* buffer = new wchar_t[buff_size];
	wcscpy_s(buffer,buff_size,arg->c_str());
	
	_wcsupr_s(buffer,buff_size);
	shared_string ret = shared_string(new std::wstring(buffer));
	delete buffer;
	return new StringVariant(ret);
}


Variant* StringFunctions::stringLower(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	shared_string arg = callInfo->getStringArg(0);
	size_t buff_size = arg->length()+1;
	wchar_t* buffer = new wchar_t[buff_size];
	wcscpy_s(buffer,buff_size,arg->c_str());
	_wcslwr_s(buffer,buff_size);
	shared_string ret = shared_string(new std::wstring(buffer));
	delete buffer;
	return new StringVariant(ret);
}

Variant* StringFunctions::stringLen(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return new Integer32Variant(callInfo->getStringArg(0)->length());
}


Variant* StringFunctions::stringLeft(CallInfo* callInfo)
{
	callInfo->validateArgCount(2,2);
	shared_string str = callInfo->getStringArg(0);
	int count = callInfo->getInt32Arg(1);
	return new StringVariant(shared_string(new std::wstring(*str,0,count)));
}
Variant* StringFunctions::stringTrimLeft(CallInfo* callInfo)
{
	callInfo->validateArgCount(2,2);
	shared_string str = callInfo->getStringArg(0);
	int count = callInfo->getInt32Arg(1);
	if(count > str->size())
		return new StringVariant(L"");
	return new StringVariant(shared_string(new std::wstring(*str,count,std::wstring::npos)));
}
Variant* StringFunctions::stringRight(CallInfo* callInfo)
{
	callInfo->validateArgCount(2,2);
	shared_string str = callInfo->getStringArg(0);
	int count = callInfo->getInt32Arg(1);
	if(count > str->size())
	{
		return new StringVariant(str);
	}
	return new StringVariant(shared_string(new std::wstring(*str,str->size()-(count),std::wstring::npos)));
}
Variant* StringFunctions::stringTrimRight(CallInfo* callInfo)
{
	callInfo->validateArgCount(2,2);
	shared_string str = callInfo->getStringArg(0);
	int count = callInfo->getInt32Arg(1);
	if(count > str->size())
	{
		return new StringVariant(L"");
	}
	return new StringVariant(shared_string(new std::wstring(*str,0,str->size()-(count))));
}


Variant* StringFunctions::stringIsDigit(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	shared_string str = callInfo->getStringArg(0);
	const wchar_t* c_str = str->c_str();

	for(int i=0;i<str->length();i++)
	{
		wchar_t chr = c_str[i];
		if(chr < L'0' || chr > L'9')
			return BooleanVariant::Get(false,true);
	}
	return BooleanVariant::Get(true,true);
}

void StringFunctions::registerFunctions(StackMachine* machine)
{
	std::shared_ptr<StringFunctions> instance(new StringFunctions);

	machine->addBuiltInFunction(L"stringupper",std::bind(&stringUpper,instance,_1));
	machine->addBuiltInFunction(L"stringlower",std::bind(&stringLower,instance,_1));
	machine->addBuiltInFunction(L"stringlen",std::bind(&stringLen,instance,_1));
	machine->addBuiltInFunction(L"stringleft",std::bind(&stringLeft,instance,_1));
	machine->addBuiltInFunction(L"stringtrimleft",std::bind(&stringTrimLeft,instance,_1));
	machine->addBuiltInFunction(L"stringright",std::bind(&stringRight,instance,_1));
	machine->addBuiltInFunction(L"stringtrimright",std::bind(&stringTrimRight,instance,_1));
	machine->addBuiltInFunction(L"stringisdigit",std::bind(&stringIsDigit,instance,_1));
}