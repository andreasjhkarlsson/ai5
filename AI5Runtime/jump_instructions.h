#include "StackMachine.h"
#include "UserFunctionVariant.h"




__forceinline void jumpLongRelative(StackMachine* machine,void* arg)
{
	machine->jumpRelative(*(int*)arg);
}

__forceinline void jumpLongRelativeIfFalse(StackMachine* machine,void* arg)
{
	Variant *v = machine->getDataStack()->pop();

	bool cond;
	if(v->getType() == Variant::BOOLEAN)
		cond = ((BooleanVariant*)v)->value;
	else
		cond = v->toBoolean();

	if(!cond)
		machine->jumpRelative(*(int*)arg);
	else
		machine->advanceCounter();
	v->release();
}

__forceinline void jumpLongRelativeIfTrue(StackMachine* machine,void* arg)
{
	Variant *v = machine->getDataStack()->pop();

	bool cond;
	if(v->getType() == Variant::BOOLEAN)
		cond = ((BooleanVariant*)v)->value;
	else
		cond = v->toBoolean();

	if(cond)
		machine->jumpRelative(*(int*)arg);
	else
		machine->advanceCounter();
	v->release();
}

__forceinline void callName(StackMachine* machine,void* arg)
{
	Variant* toCall = machine->getNameStorage()->getNameFromIndex(*(int*)arg)->findNearest();
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