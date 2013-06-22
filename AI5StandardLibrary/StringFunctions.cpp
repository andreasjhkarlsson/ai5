#include "StringFunctions.h"
#include "..\AI5Runtime\StringVariant.h"
#include "..\AI5Runtime\Integer32Variant.h"
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


Variant* StringFunctions::stringupper(Variant** args,int argsSize)
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


Variant* StringFunctions::stringlower(Variant** args,int argsSize)
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

Variant* StringFunctions::stringlen(Variant** args,int argsSize)
{
	return new Integer32Variant(args[0]->toString()->length());
}


void StringFunctions::registerFunctions(StackMachine* machine)
{
	std::shared_ptr<StringFunctions> instance(new StringFunctions);

	machine->addBuiltInFunction(L"stringupper",std::bind(&stringupper,instance,_1,_2));
	machine->addBuiltInFunction(L"stringlower",std::bind(&stringlower,instance,_1,_2));
	machine->addBuiltInFunction(L"stringlen",std::bind(&stringlen,instance,_1,_2));


}