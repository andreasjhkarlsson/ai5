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

__forceinline void loadArgument(StackMachine* machine, NameIdentifier arg)
{
	// This is exactly the same thing.
	assignLocal(machine,arg);
}

__forceinline void loadByRefArgument(StackMachine* machine,NameIdentifier arg)
{
	

	if(machine->getDataStack()->top()->getLastName() != nullptr)
	{
		Variant* var = machine->getDataStack()->pop();
		machine->addNameToLocalScope(arg,var->getLastName());
		machine->advanceCounter();
	}
	else
	{
		loadArgument(machine,arg);
	}

}

__forceinline void assignNearest(StackMachine* machine,NameIdentifier arg)
{
	Variant* var = machine->getDataStack()->pop();
	
	machine->setNearest(arg,var);

	var->release();
	machine->advanceCounter();
}

__forceinline void assignNearestConst(StackMachine* machine,NameIdentifier arg)
{
	Variant* var = machine->getDataStack()->pop();
	
	machine->setNearest(arg,var,true);

	var->release();
	machine->advanceCounter();
}

__forceinline void assignLocalConst(StackMachine* machine,NameIdentifier arg)
{
	Variant* var = machine->getDataStack()->pop();
	
	machine->setLocal(arg,var,true);

	var->release();
	machine->advanceCounter();
}

__forceinline void assignGlobalConst(StackMachine* machine,NameIdentifier arg)
{
	Variant* var = machine->getDataStack()->pop();
	
	machine->setGlobal(arg,var,true);

	var->release();
	machine->advanceCounter();
}


__forceinline void concatStrings(StackMachine* machine)
{

	Variant* arg2 = machine->getDataStack()->pop();
	Variant* arg1 = machine->getDataStack()->pop();

	shared_string result = shared_string(new std::wstring(*arg1->toString() + *arg2->toString()));

	arg1->release();
	arg2->release();

	machine->getDataStack()->push(new StringVariant(result));

	machine->advanceCounter();
}