#include "StringFunctions.h"
#include "..\AI5Runtime\StringVariant.h"
#include "..\AI5Runtime\Integer32Variant.h"
#include "..\AI5Runtime\BooleanVariant.h"
#include "..\AI5Runtime\StackMachine.h"
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


Variant* StringFunctions::stringUpper(Variant** args,int argsSize)
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


Variant* StringFunctions::stringLower(Variant** args,int argsSize)
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

Variant* StringFunctions::stringLen(Variant** args,int argsSize)
{
	return new Integer32Variant(args[0]->toString()->length());
}


Variant* StringFunctions::stringLeft(Variant** args,int argsSize)
{
	validateArgCount(argsSize,2,2);
	shared_string str = args[0]->toString();
	int count = args[1]->toInteger32();
	return new StringVariant(shared_string(new std::wstring(*str,0,count)));
}
Variant* StringFunctions::stringTrimLeft(Variant** args,int argsSize)
{
	validateArgCount(argsSize,2,2);
	shared_string str = args[0]->toString();
	int count = args[1]->toInteger32();
	if(count > str->size())
		return new StringVariant(L"");
	return new StringVariant(shared_string(new std::wstring(*str,count,std::wstring::npos)));
}
Variant* StringFunctions::stringRight(Variant** args,int argsSize)
{
	validateArgCount(argsSize,2,2);
	shared_string str = args[0]->toString();
	int count = args[1]->toInteger32();
	if(count > str->size())
	{
		return new StringVariant(str);
	}
	return new StringVariant(shared_string(new std::wstring(*str,str->size()-(count),std::wstring::npos)));
}
Variant* StringFunctions::stringTrimRight(Variant** args,int argsSize)
{
	validateArgCount(argsSize,2,2);
	shared_string str = args[0]->toString();
	int count = args[1]->toInteger32();
	if(count > str->size())
	{
		return new StringVariant(L"");
	}
	return new StringVariant(shared_string(new std::wstring(*str,0,str->size()-(count))));
}


Variant* StringFunctions::stringIsDigit(Variant** args,int argsSize)
{
	validateArgCount(argsSize,1,1);
	shared_string str = args[0]->toString();
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

	machine->addBuiltInFunction(L"stringupper",std::bind(&stringUpper,instance,_1,_2));
	machine->addBuiltInFunction(L"stringlower",std::bind(&stringLower,instance,_1,_2));
	machine->addBuiltInFunction(L"stringlen",std::bind(&stringLen,instance,_1,_2));
	machine->addBuiltInFunction(L"stringleft",std::bind(&stringLeft,instance,_1,_2));
	machine->addBuiltInFunction(L"stringtrimleft",std::bind(&stringTrimLeft,instance,_1,_2));
	machine->addBuiltInFunction(L"stringright",std::bind(&stringRight,instance,_1,_2));
	machine->addBuiltInFunction(L"stringtrimright",std::bind(&stringTrimRight,instance,_1,_2));
	machine->addBuiltInFunction(L"stringisdigit",std::bind(&stringIsDigit,instance,_1,_2));
}