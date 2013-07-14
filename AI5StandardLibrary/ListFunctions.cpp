#include "ListFunctions.h"
#include "..\AI5Runtime\ListVariant.h"
#include "..\AI5Runtime\NullVariant.h"
#include "..\AI5Runtime\StackMachineThread.h"
#include "..\AI5Runtime\CallInfo.h"
#include <functional>
#include <memory>
using namespace std::placeholders;

ListFunctions::ListFunctions(void)
{
}


ListFunctions::~ListFunctions(void)
{
}


void ListFunctions::registerFunctions(StackMachineThread* machine)
{
	std::shared_ptr<ListFunctions> instance(new ListFunctions);

	machine->addBuiltInFunction(L"ubound",std::bind(&ubound,instance,_1));
	machine->addBuiltInFunction(L"arrayadd",std::bind(&arrayadd,instance,_1));
}


Variant* ListFunctions::ubound(CallInfo* callInfo)
{

	callInfo->validateArgCount(1,1);

	Variant* var = callInfo->getArg(0);
	Variant* result;

	result = new Integer32Variant((int)var->cast<ListVariant>()->size());

	return result;
}


Variant* ListFunctions::arrayadd(CallInfo* callInfo)
{
	callInfo->validateArgCount(2,2);
	ListVariant* listArg = callInfo->getArg(0)->cast<ListVariant>();
	Variant* elementArg = callInfo->getArg(1);


	listArg->addElement(elementArg);

	NullVariant::Instance.addRef();
	return &NullVariant::Instance;
}
