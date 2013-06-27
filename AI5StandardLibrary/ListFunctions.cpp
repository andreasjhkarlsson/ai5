#include "ListFunctions.h"
#include "..\AI5Runtime\ListVariant.h"
#include "..\AI5Runtime\NullVariant.h"
#include "..\AI5Runtime\StackMachine.h"
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

	machine->addBuiltInFunction(L"ubound",std::bind(&ubound,instance,_1,_2));
	machine->addBuiltInFunction(L"arrayadd",std::bind(&arrayadd,instance,_1,_2));
}


Variant* ListFunctions::ubound(Variant** args,int argsSize)
{
	Variant* var = args[0];
	Variant* result;


	if(var->isListType())
	{
		result = new Integer32Variant((int)static_cast<ListVariant*>(var)->size());
	}
	else
	{
		result = &NullVariant::Instance;
		result->addRef();
	}

	return result;
}


Variant* ListFunctions::arrayadd(Variant** args,int argsSize)
{
	Variant* listArg = args[0];
	Variant* elementArg = args[1];

	if(!listArg->isListType())
		throw RuntimeError(L"Can only add element to list type.");

	static_cast<ListVariant*>(listArg)->addElement(elementArg);

	NullVariant::Instance.addRef();
	return &NullVariant::Instance;
}
