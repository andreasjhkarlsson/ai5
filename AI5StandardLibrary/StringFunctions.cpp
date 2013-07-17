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


VariantReference<> StringFunctions::stringUpper(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	shared_string arg = callInfo->getStringArg(0);
	shared_string ret = shared_string(new UnicodeString(arg->toUpper()));
	return new StringVariant(ret);
}


VariantReference<> StringFunctions::stringLower(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	shared_string arg = callInfo->getStringArg(0);
	shared_string ret = shared_string(new UnicodeString(arg->toLower()));
	return new StringVariant(ret);
}

VariantReference<> StringFunctions::stringLen(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return callInfo->getStringArg(0)->length();
}


VariantReference<> StringFunctions::stringLeft(CallInfo* callInfo)
{
	callInfo->validateArgCount(2,2);
	shared_string str = callInfo->getStringArg(0);
	int count = callInfo->getInt32Arg(1);
	shared_string ret = shared_string(new UnicodeString(L""));
	str->extract(0,count,*ret);
	return new StringVariant(ret);
}

VariantReference<> StringFunctions::stringTrimLeft(CallInfo* callInfo)
{
	callInfo->validateArgCount(2,2);
	shared_string str = callInfo->getStringArg(0);
	int count = callInfo->getInt32Arg(1);
	if(count > str->countChar32())
		return new StringVariant(L"");
	shared_string ret = shared_string(new UnicodeString(L""));
	str->extract(count,str->length()-count,*ret);
	return new StringVariant(ret);
}
VariantReference<> StringFunctions::stringRight(CallInfo* callInfo)
{
	callInfo->validateArgCount(2,2);
	shared_string str = callInfo->getStringArg(0);
	int count = callInfo->getInt32Arg(1);
	if(count > str->length())
	{
		return new StringVariant(str);
	}
	shared_string ret =  shared_string(new UnicodeString(L""));
	str->extract(str->length()-count,count,*ret);
	return new StringVariant(ret);
}
VariantReference<> StringFunctions::stringTrimRight(CallInfo* callInfo)
{
	callInfo->validateArgCount(2,2);
	shared_string str = callInfo->getStringArg(0);
	int count = callInfo->getInt32Arg(1);
	if(count > str->length())
	{
		return new StringVariant(L"");
	}
	shared_string ret = shared_string(new UnicodeString(L""));
	str->extract(0,str->length()-count,*ret);
	return new StringVariant(ret);
}


VariantReference<> StringFunctions::stringIsDigit(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	shared_string str = callInfo->getStringArg(0);
	const wchar_t* c_str = str->getTerminatedBuffer();

	for(int i=0;i<str->length();i++)
	{

		wchar_t chr = c_str[i];
		if(chr < L'0' || chr > L'9')
			return false;
	}
	return true;
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