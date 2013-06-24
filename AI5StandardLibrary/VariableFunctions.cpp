#include "VariableFunctions.h"
#include "..\AI5Runtime\StringVariant.h"
#include "..\AI5Runtime\Integer32Variant.h"
#include "..\AI5Runtime\BinaryVariant.h"
#include "..\AI5Runtime\BooleanVariant.h"
#include "..\AI5Runtime\StackMachine.h"
#include <string>
#include <wchar.h>
#include <functional>
#include <memory>
#include <Windows.h>
using namespace std::placeholders;

VariableFunctions::VariableFunctions(void)
{
}


VariableFunctions::~VariableFunctions(void)
{
}



void VariableFunctions::registerFunctions(StackMachine* machine)
{
	std::shared_ptr<VariableFunctions> instance(new VariableFunctions);

	machine->addBuiltInFunction(L"stringtobinary",std::bind(&stringToBinary,instance,_1,_2));
	machine->addBuiltInFunction(L"binarytostring",std::bind(&binaryToString,instance,_1,_2));
	machine->addBuiltInFunction(L"isfloat",std::bind(&isFloat,instance,_1,_2));
	machine->addBuiltInFunction(L"isstring",std::bind(&isString,instance,_1,_2));
	machine->addBuiltInFunction(L"isarray",std::bind(&isArray,instance,_1,_2));
	machine->addBuiltInFunction(L"ishashmap",std::bind(&isHashMap,instance,_1,_2));
	machine->addBuiltInFunction(L"isbool",std::bind(&isBool,instance,_1,_2));
	machine->addBuiltInFunction(L"string",std::bind(&isString,instance,_1,_2));
}


Variant* VariableFunctions::stringToBinary(Variant** args,int argCount)
{
	validateArgCount(argCount,1,2);
	shared_string str = args[0]->toString();
	int flag = 1;
	if(argCount == 2)
		flag = args[1]->toInteger32();

	if(flag < 0 || flag > 4)
		throw RuntimeError(L"Flag must be between 0 and 4");
	
	shared_binary res = shared_binary(new std::vector<char>);

	switch(flag)
	{
	case 1: // ANSI
		{
			size_t required_size = WideCharToMultiByte(CP_ACP,0,str->c_str(),str->length(),NULL,0,NULL,NULL);
			res->resize(required_size);
			WideCharToMultiByte(CP_ACP,0,str->c_str(),str->length(),&(*res)[0],required_size,NULL,NULL);
		}
		break;
	case 2: // UTF-16 little endian.
		{
		res->resize(str->length()*2);
		memcpy(&(*res)[0],str->c_str(),str->length()*2);
		}
		break;
	case 3: // UTF-16 big endian.
		{
		res->resize(str->length()*2);
		memcpy(&(*res)[0],str->c_str(),str->length()*2);
		swapUtf16Endiness((wchar_t*)&(*res)[0],res->size()/2);
		}
		break;
	case 4: // UTF-8.
		{
		size_t required_size = WideCharToMultiByte(CP_UTF8,0,str->c_str(),str->size(),NULL,0,NULL,NULL);
		res->resize(required_size);
		WideCharToMultiByte(CP_UTF8,0,str->c_str(),str->length(),&(*res)[0],required_size,NULL,NULL);
		}
		break;
	}

	return new BinaryVariant(res);

}

void VariableFunctions::swapUtf16Endiness(wchar_t* binary,int size)
{
	for(int i=0;i<size;i++)
	{
		char high = (binary[i]>>8)&0xFF;
		char low = binary[i]&0xFF;
		binary[i] = (low<<8)|high;
	}
}

Variant* VariableFunctions::binaryToString(Variant** args,int argCount)
{
	validateArgCount(argCount,1,2);

	if(!args[0]->isBinaryType())
		throw RuntimeError(L"Type error! BinaryToString requires binary type");

	shared_binary binary = static_cast<BinaryVariant*>(args[0])->getValue();

	int flag = 1;
	if(argCount == 2)
		flag = args[1]->toInteger32();

	if(flag < 0 || flag > 4)
		throw RuntimeError(L"Flag must be between 0 and 4");

	shared_string result = nullptr;


	switch(flag)
	{
	case 1:
		{
		size_t required_size = MultiByteToWideChar(CP_ACP,0,&(*binary)[0],binary->size(),NULL,0);
		std::vector<wchar_t> buffer(required_size);
		MultiByteToWideChar(CP_ACP,0,&(*binary)[0],binary->size(),&buffer[0],required_size);
		result = shared_string(new std::wstring(&buffer[0],required_size));
		}
		break;
	case 2:
		{
			result = shared_string(new std::wstring((wchar_t*)&(*binary)[0],binary->size()/2));
		}
		break;	
	case 3:
		{
			std::vector<char> buffer = *binary;
			swapUtf16Endiness((wchar_t*)&buffer[0],buffer.size()/2);
			result = shared_string(new std::wstring((wchar_t*)&buffer[0],buffer.size()/2));
		}
		break;
	case 4:
		{
		size_t required_size = MultiByteToWideChar(CP_UTF8,0,&(*binary)[0],binary->size(),NULL,0);
		std::vector<wchar_t> buffer(required_size);
		MultiByteToWideChar(CP_UTF8,0,&(*binary)[0],binary->size(),&buffer[0],required_size);
		result = shared_string(new std::wstring(&buffer[0],required_size));
		}
		break;

	}

	return new StringVariant(result);
}



Variant* VariableFunctions::isFloat(Variant** args,int argCount)
{
	validateArgCount(argCount,1,1);
	return BooleanVariant::Get(args[0]->isFloatingType(),true);
	
}
Variant* VariableFunctions::isString(Variant** args,int argCount)
{
	validateArgCount(argCount,1,1);
	return BooleanVariant::Get(args[0]->isStringType(),true);
	
}
Variant* VariableFunctions::isArray(Variant** args,int argCount)
{
	validateArgCount(argCount,1,1);
	return BooleanVariant::Get(args[0]->isListType(),true);
	
}
Variant* VariableFunctions::isHashMap(Variant** args,int argCount)
{
	validateArgCount(argCount,1,1);
	return BooleanVariant::Get(args[0]->isHashMap(),true);
	
}
Variant* VariableFunctions::isBool(Variant** args,int argCount)
{
	validateArgCount(argCount,1,1);
	return BooleanVariant::Get(args[0]->isBooleanType(),true);
	
}
Variant* VariableFunctions::ToString(Variant** args,int argCount)
{
	validateArgCount(argCount,1,1);
	return new StringVariant(args[0]->toString());	
}