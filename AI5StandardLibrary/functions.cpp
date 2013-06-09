#include <iostream>
#include <Windows.h>
#include "..\AI5Runtime\StackMachine.h"
#include "functions.h"
#include "..\AI5Runtime\Integer64Variant.h"
#include "..\AI5Runtime\NullVariant.h"
#include "..\AI5Runtime\ListVariant.h"

namespace AI5StandardLibrary
{

void hello(StackMachine* machine)
{
	machine->getDataStack()->pop(); // <-- self
	std::cout << "Hello there!" << std::endl;
	machine->getDataStack()->pushNull();
}

void sleep(StackMachine* machine)
{
	Variant* var = machine->getDataStack()->pop();
	machine->getDataStack()->pop(); // <-- self

	int sleepDelay = var->toInteger32();
	var->release();
	Sleep(sleepDelay);
	machine->getDataStack()->pushNull();
}

void printline(StackMachine* machine)
{
	Variant* var = machine->getDataStack()->pop();
	machine->getDataStack()->pop();

	var->format(std::wcout);
	std::wcout << std::endl;

	var->release();
	machine->getDataStack()->pushNull();
}


void getint(StackMachine* machine)
{
	machine->getDataStack()->pop();

	__int64 res;
	std::cin >> res;

	machine->getDataStack()->push(new Integer64Variant(res));
}

void stringlen(StackMachine* machine)
{
	Variant* var = machine->getDataStack()->pop();
	machine->getDataStack()->pop();

	size_t result = var->toString()->length();

	var->release();

	machine->getDataStack()->push(new Integer64Variant(result));

}



void ubound(StackMachine* machine)
{
	Variant* var = machine->getDataStack()->pop();
	Variant* result;
	machine->getDataStack()->pop();

	if(var->isListType())
	{
		result = new Integer32Variant((int)static_cast<ListVariant*>(var)->size());
	}
	else
	{
		result = &NullVariant::Instance;
		result->addRef();
	}

	var->release();

	machine->getDataStack()->push(result);

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
