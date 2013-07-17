#include "ListFunctions.h"
#include "..\AI5Runtime\ListVariant.h"
#include "..\AI5Runtime\NullVariant.h"
#include "..\AI5Runtime\StackMachine.h"
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


void ListFunctions::registerFunctions(StackMachine* machine)
{
	std::shared_ptr<ListFunctions> instance(new ListFunctions);

	machine->addBuiltInFunction(L"ubound",std::bind(&ubound,instance,_1));
	machine->addBuiltInFunction(L"arrayadd",std::bind(&arrayadd,instance,_1));
}


VariantReference<> ListFunctions::ubound(CallInfo* callInfo)
{

	callInfo->validateArgCount(1,1);

	VariantReference<ListVariant> var = callInfo->getArg(0).cast<ListVariant>();
	VariantReference<> result;

	result = (int)var->size();

	return result;
}


VariantReference<> ListFunctions::arrayadd(CallInfo* callInfo)
{
	callInfo->validateArgCount(2,2);
	VariantReference<ListVariant> listArg = callInfo->getArg(0).cast<ListVariant>();
	VariantReference<> elementArg = callInfo->getArg(1);


	listArg->addElement(elementArg);


	return VariantReference<>::NullReference();
}
