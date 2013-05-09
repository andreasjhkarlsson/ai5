#include "StackMachine.h"

__forceinline void noop(StackMachine* machine)
{
	machine->advanceCounter();
}

__forceinline void terminate(StackMachine* machine)
{
	machine->terminate();
}

__forceinline void createGlobal(StackMachine* machine, void* arg)
{
	machine->createGlobal(*(int*)arg);
	machine->advanceCounter();
}

__forceinline void createLocal(StackMachine* machine, void* arg)
{
	machine->createLocal(*(int*)arg);
	machine->advanceCounter();
}

__forceinline void assignName(StackMachine* machine,void*arg)
{
	Variant* var = machine->getDataStack()->pop();
	machine->getName(*(int*)arg)->setNearest(var);
	var->release();
	machine->advanceCounter();
}