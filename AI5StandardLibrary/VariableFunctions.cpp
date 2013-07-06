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
	machine->addBuiltInFunction(L"string",std::bind(&isString,instance,_1));
}


Variant* VariableFunctions::stringToBinary(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,2);
	shared_string str = callInfo->getStringArg(0);
	int flag = callInfo->getInt32Arg(1,1);


	if(flag < 0 || flag > 4)
		throw RuntimeError(L"Flag must be between 0 and 4");
	
	shared_binary res = Encode::encode(str->getTerminatedBuffer(),str->length(),flag);

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

Variant* VariableFunctions::binaryToString(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,2);

	if(!callInfo->getArg(0)->isBinaryType())
		throw RuntimeError(L"Type error! BinaryToString requires binary type");

	shared_binary binary = callInfo->getArg(0)->cast<BinaryVariant>()->getValue();

	int flag = callInfo->getInt32Arg(1,1);

	if(flag < 0 || flag > 4)
		throw RuntimeError(L"Flag must be between 0 and 4");

	shared_string result = Encode::decode(&(*binary)[0],binary->size(),flag);

	return new StringVariant(result);
}



Variant* VariableFunctions::isFloat(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return BooleanVariant::Get(callInfo->getArg(0)->isFloatingType(),true);
	
}
Variant* VariableFunctions::isString(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return BooleanVariant::Get(callInfo->getArg(0)->isStringType(),true);
	
}
Variant* VariableFunctions::isArray(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return BooleanVariant::Get(callInfo->getArg(0)->isListType(),true);
	
}
Variant* VariableFunctions::isHashMap(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return BooleanVariant::Get(callInfo->getArg(0)->isHashMap(),true);
	
}
Variant* VariableFunctions::isBool(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return BooleanVariant::Get(callInfo->getArg(0)->isBooleanType(),true);
	
}
Variant* VariableFunctions::ToString(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	return new StringVariant(callInfo->getArg(0)->toString());	
}