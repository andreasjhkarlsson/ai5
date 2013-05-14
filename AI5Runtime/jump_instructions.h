#include "StackMachine.h"
#include "UserFunctionVariant.h"




__forceinline void jumpLongRelative(StackMachine* machine,unsigned int arg)
{
	machine->jumpRelative(arg);
}

__forceinline void jumpLongRelativeIfFalse(StackMachine* machine,unsigned int arg)
{
	Variant *v = machine->getDataStack()->pop();

	bool cond;
	if(v->getType() == Variant::BOOLEAN)
		cond = ((BooleanVariant*)v)->value;
	else
		cond = v->toBoolean();

	if(!cond)
		machine->jumpRelative(arg);
	else
		machine->advanceCounter();
	v->release();
}


__forceinline void jumpShortRelativeIfFalse(StackMachine* machine,char arg)
{
	jumpLongRelativeIfFalse(machine,arg);
}


__forceinline void jumpShortRelative(StackMachine* machine,char arg)
{
	jumpLongRelative(machine,arg);
}

__forceinline void jumpLongRelativeIfTrue(StackMachine* machine,unsigned int arg)
{
	Variant *v = machine->getDataStack()->pop();

	bool cond;
	if(v->getType() == Variant::BOOLEAN)
		cond = ((BooleanVariant*)v)->value;
	else
		cond = v->toBoolean();

	if(cond)
		machine->jumpRelative(arg);
	else
		machine->advanceCounter();
	v->release();
}

__forceinline void callFunction(StackMachine* machine,unsigned int numberOfArgs)
{
	// The actual object resides below the arguments on the stack. 
	// It's up the the function to pop arguments + function from stack.
	Variant* toCall = machine->getDataStack()->get(numberOfArgs);

	if(toCall->getType() == Variant::FUNCTION_VAR)
	{
		((FunctionVariant*)toCall)->call(machine);
		
	}
	else
	{
		// Throw error!!
	}
}

__forceinline void ret(StackMachine* machine)
{
	int returnAddress = machine->popCallFrame();
	machine->jumpAbsolute(returnAddress);
}