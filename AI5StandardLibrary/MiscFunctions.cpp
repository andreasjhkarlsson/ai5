#include "MiscFunctions.h"
#include "..\AI5Runtime\NullVariant.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\CallInfo.h"
#include <functional>
#include <memory>
using namespace std::placeholders;

MiscFunctions::MiscFunctions(void)
{
}


MiscFunctions::~MiscFunctions(void)
{
}


void MiscFunctions::registerFunctions(StackMachine* machine)
{
	std::shared_ptr<MiscFunctions> instance(new MiscFunctions);

	machine->addBuiltInFunction(L"seterror",std::bind(&setError,instance,_1));
	machine->addBuiltInFunction(L"setextended",std::bind(&setExtended,instance,_1));
}



Variant* MiscFunctions::setError(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,3);

	callInfo->setError(callInfo->getInt32Arg(0));
	if(callInfo->getArgCount() >= 2)
	{
		callInfo->setExtended(callInfo->getInt32Arg(1));
	}

	if(callInfo->getArgCount() >= 3)
	{
		Variant* ret = callInfo->getArg(2);
		ret->addRef();
		return ret;
	}

	return nullptr;

}


Variant* MiscFunctions::setExtended(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,2);

	callInfo->setExtended(callInfo->getInt32Arg(0));


	if(callInfo->getArgCount() >= 2)
	{
		Variant* ret = callInfo->getArg(1);
		ret->addRef();
		return ret;
	}

	return nullptr;

}