#include "StackMachine.h"
#include "IntegerVariant.h"

__forceinline void pushSmallInteger(StackMachine* machine,void* arg)
{
	Variant *v = IntegerVariant::BYTE_TABLE[(*(char*)arg)+128];
	v->addRef();
	machine->getDataStack()->push(v);
	machine->advanceCounter();
}


__forceinline void pushLargeInteger(StackMachine* machine,void* arg)
{
	machine->getDataStack()->push(machine->getVariantFactory()->create<IntegerVariant,__int64>(Variant::INTEGER,*(int*)arg));
	machine->advanceCounter();
}


__forceinline void pop(StackMachine* machine)
{
	machine->getDataStack()->pop()->release();
	machine->advanceCounter();
}

__forceinline void doubleTop(StackMachine* machine)
{
	DataStack *stack = machine->getDataStack();
	stack->push(stack->top());
	stack->top()->addRef();
	machine->advanceCounter();
}


__forceinline void pushName(StackMachine* machine,void* arg)
{
	Name* name = machine->getNameStorage()->getNameFromIndex(*(int*)arg);
	Variant* var = name->findNearest();
	var->addRef();
	machine->getDataStack()->push(var);
	machine->advanceCounter();
}