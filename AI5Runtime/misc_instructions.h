#include "StackMachine.h"
#include "NullVariant.h"

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
	machine->getNameStorage()->getNameFromIndex(*(int*)arg)->setGlobal(&NullVariant::Instance);
	machine->advanceCounter();
}

__forceinline void createLocal(StackMachine* machine, void* arg)
{
	machine->getNameStorage()->getNameFromIndex(*(int*)arg)->pushLocal(&NullVariant::Instance);
	machine->advanceCounter();
}

__forceinline void assignName(StackMachine* machine,void*arg)
{
	Variant* var = machine->getDataStack()->pop();
	machine->getNameStorage()->getNameFromIndex(*(int*)arg)->setNearest(var);
	var->release();
	machine->advanceCounter();
}