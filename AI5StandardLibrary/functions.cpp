#include <iostream>
#include <Windows.h>
#include "..\AI5Runtime\StackMachine.h"
#include "functions.h"
#include "..\AI5Runtime\Integer64Variant.h"

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

	var->print();

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

	int result = var->toString()->length();

	var->release();

	machine->getDataStack()->push(new Integer64Variant(result));

}

}
