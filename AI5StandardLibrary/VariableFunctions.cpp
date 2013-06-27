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
	
	shared_binary res = Encode::encode(str->c_str(),str->length(),flag);

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

	shared_string result = Encode::decode(&(*binary)[0],binary->size(),flag);

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