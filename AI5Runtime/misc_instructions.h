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

__forceinline void assignGlobal(StackMachine* machine, NameIdentifier arg)
{
	Variant* var = machine->getDataStack()->pop();
	
	machine->setGlobal(arg,var);

	var->release();
	machine->advanceCounter();
}

__forceinline void assignLocal(StackMachine* machine, NameIdentifier arg)
{
	Variant* var = machine->getDataStack()->pop();
	
	machine->setLocal(arg,var);

	var->release();
	machine->advanceCounter();
}

__forceinline void assignNearest(StackMachine* machine,NameIdentifier arg)
{
	Variant* var = machine->getDataStack()->pop();
	
	machine->setNearest(arg,var);

	var->release();
	machine->advanceCounter();
}