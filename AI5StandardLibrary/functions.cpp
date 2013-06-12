#include <iostream>
#include <Windows.h>
#include "..\AI5Runtime\StackMachine.h"
#include "functions.h"
#include "..\AI5Runtime\Integer64Variant.h"
#include "..\AI5Runtime\NullVariant.h"
#include "..\AI5Runtime\ListVariant.h"

namespace AI5StandardLibrary
{

Variant* hello(Variant** args,int argsSize)
{
	std::cout << "Hello there!" << std::endl;
	NullVariant::Instance.addRef();
	return &NullVariant::Instance;
}

Variant* sleep(Variant** args,int argsSize)
{
	Variant* var = args[0];

	int sleepDelay = var->toInteger32();

	Sleep(sleepDelay);
	NullVariant::Instance.addRef();
	return &NullVariant::Instance;
}

Variant* printline(Variant** args,int argsSize)
{
	Variant* var = args[0];

	var->format(std::wcout);
	std::wcout << std::endl;

	NullVariant::Instance.addRef();
	return &NullVariant::Instance;
}


Variant* getint(Variant** args,int argsSize)
{
	__int64 res;
	std::cin >> res;
	return new Integer64Variant(res);
}

Variant* stringlen(Variant** args,int argsSize)
{
	Variant* var = args[0];

	size_t result = var->toString()->length();

	return new Integer64Variant(result);
}



Variant* ubound(Variant** args,int argsSize)
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


Variant* arrayadd(Variant** args,int argsSize)
{
	Variant* listArg = args[0];
	Variant* elementArg = args[1];

	if(!listArg->isListType())
		throw RuntimeError(L"Can only add element to list type.");

	static_cast<ListVariant*>(listArg)->addElement(elementArg);

	NullVariant::Instance.addRef();
	return &NullVariant::Instance;
}

Variant* Macros::MyPID(StackMachine*)
{
	return new Integer64Variant(GetCurrentProcessId());
}


Variant* Macros::StackSize(StackMachine* machine)
{
	return new Integer32Variant((int)machine->getDataStack()->size());
}

}
