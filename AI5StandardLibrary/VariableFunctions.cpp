#include "VariableFunctions.h"
#include "..\AI5Runtime\StringVariant.h"
#include "..\AI5Runtime\Integer32Variant.h"
#include "..\AI5Runtime\BinaryVariant.h"
#include "..\AI5Runtime\StackMachine.h"
#include <string>
#include <wchar.h>
#include <functional>
#include <memory>
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
	machine->addBuiltInFunction(L"binaryToString",std::bind(&binaryToString,instance,_1,_2));
}


Variant* VariableFunctions::stringToBinary(Variant** args,int argCount)
{
	validateArgCount(argCount,1,2);
	shared_string str = args[0]->toString();
	int flag = 1;
	if(argCount == 2)
		flag = args[1]->toInteger32();

	
	shared_binary res = shared_binary(new std::vector<char>);

	switch(flag)
	{
	case 1: // ANSI
		break;
	case 2: // UTF-16 little endian.
		break;
	case 3: // UTF-16 big endian.
		break;
	case 4: // UTF-8.
		break;

	}

	return new BinaryVariant(res);

}

Variant* VariableFunctions::binaryToString(Variant** args,int argCount)
{
	return nullptr;
}