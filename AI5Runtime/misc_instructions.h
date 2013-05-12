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

__forceinline void createGlobal(StackMachine* machine, unsigned int arg)
{
	machine->getNameStorage()->getNameFromIndex(arg)->setGlobal(&NullVariant::Instance);
	machine->advanceCounter();
}

__forceinline void createLocal(StackMachine* machine, unsigned int arg)
{
	Name* name = machine->getNameStorage()->getNameFromIndex(arg);
	name->pushLocal(&NullVariant::Instance);
	machine->getCurrentCallFrame()->attachName(name);
	machine->advanceCounter();
}

__forceinline void assignName(StackMachine* machine,unsigned int arg)
{
	Variant* var = machine->getDataStack()->pop();
	Name* name = machine->getNameStorage()->getNameFromIndex(arg);
	//name->setNearest(var);
	
	if(!name->hasAny())
	{
		CallFrame* frame = machine->getCurrentCallFrame();
		if(frame != nullptr)
		{
			name->pushLocal(var);
			frame->attachName(name);
		}
		else
		{
			name->setGlobal(var);
		}
	}
	else
	{
		name->setNearest(var);
	}

	var->release();
	machine->advanceCounter();
}