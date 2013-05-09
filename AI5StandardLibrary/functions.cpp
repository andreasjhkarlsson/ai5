#include <iostream>
#include <Windows.h>
#include "..\AI5Runtime\StackMachine.h"
#include "functions.h"


namespace AI5StandardLibrary
{

void hello(StackMachine* machine)
{
	std::cout << "Hello there!" << std::endl;
	machine->getDataStack()->pushNull();
}


void sleep(StackMachine* machine)
{
	Variant* var = machine->getDataStack()->pop();
	int sleepDelay = var->toInteger();
	var->release();
	Sleep(sleepDelay);
	machine->getDataStack()->pushNull();
}


}
