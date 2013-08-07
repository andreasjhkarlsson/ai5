#include "VariableFunctions.h"
#include "..\AI5Runtime\StringVariant.h"
#include "..\AI5Runtime\Integer32Variant.h"
#include "..\AI5Runtime\BinaryVariant.h"
#include "..\AI5Runtime\BooleanVariant.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\CallInfo.h"
#include <string>
#include <wchar.h>
#include <functional>
#include <memory>
#include <Windows.h>
#include "..\AI5Runtime\encode.h"
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

	machine->addBuiltInFunction(L"stringtobinary",std::bind(&stringToBinary,instance,_1));
	machine->addBuiltInFunction(L"binarytostring",std::bind(&binaryToString,instance,_1));
	machine->addBuiltInFunction(L"isfloat",std::bind(&isFloat,instance,_1));
	machine->addBuiltInFunction(L"isstring",std::bind(&isString,instance,_1));
	machine->addBuiltInFunction(L"isarray",std::bind(&isArray,instance,_1));
	machine->addBuiltInFunction(L"ishashmap",std::bind(&isHashMap,instance,_1));
	machine->addBuiltInFunction(L"isbool",std::bind(&isBool,instance,_1));
	machine->addBuiltInFunction(L"string",std::bind(&toString,instance,_1));
	machine->addBuiltInFunction(L"ptr",std::bind(&toPointer,instance,_1));
}


VariantReference<> VariableFunctions::stringToBinary(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,2);
	shared_string str = callInfo->getStringArg(0);
	int flag = callInfo->getInt32Arg(1,1);


	if(flag < 0 || flag > 4)
		throw RuntimeError(L"Flag must be between 0 and 4");
	
	shared_binary res = Encode::encode(str->getTerminatedBuffer(),str->length(),flag);

	return BinaryVariant::Create(res);

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

VariantReference<> VariableFunctions::binaryToString(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,2);

	if(!callInfo->getArg(0).isBinaryType())
		throw RuntimeError(L"Type error! BinaryToString requires binary type");

	shared_binary binary = callInfo->getArg(0).cast<BinaryVariant>()->getValue();

	int flag = callInfo->getInt32Arg(1,1);

	if(flag < 0 || flag > 4)
		throw RuntimeError(L"Flag must be between 0 and 4");

	shared_string result = Encode::decode(&(*binary)[0],binary->size(),flag);

	return StringVariant::Create(result);
}



VariantReference<> VariableFunctions::isFloat(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return callInfo->getArg(0).isFloatingType();
	
}
VariantReference<> VariableFunctions::isString(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return callInfo->getArg(0).isStringType();
	
}
VariantReference<> VariableFunctions::isArray(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return callInfo->getArg(0).isListType();
	
}
VariantReference<> VariableFunctions::isHashMap(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return callInfo->getArg(0).isHashMap();
	
}
VariantReference<> VariableFunctions::isBool(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return callInfo->getArg(0).isBooleanType();
	
}
VariantReference<> VariableFunctions::toString(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return StringVariant::Create(callInfo->getArg(0).toString());	
}

VariantReference<> VariableFunctions::toPointer(CallInfo* callInfo)
{
	void* ptr = nullptr;
	if(sizeof(void*) == 8)
	{
		ptr = reinterpret_cast<void*>(callInfo->getInt64Arg(0));
	}
	else if(sizeof(void*) == 4)
	{
		ptr = reinterpret_cast<void*>(callInfo->getInt32Arg(0));
	}
	else
	{
		throw RuntimeError(L"Unknown pointer platform!!"); // What?
	}

	return VariantReference<>::PointerReference(ptr);
}