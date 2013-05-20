#include "StackMachine.h"
#include "IntegerVariant.h"
#include "UserFunctionVariant.h"

__forceinline void pushSmallInteger(StackMachine* machine,char arg)
{
	Variant *v = IntegerVariant::BYTE_TABLE[arg+128];
	v->addRef();
	machine->getDataStack()->push(v);
	machine->advanceCounter();
}


__forceinline void pushLargeInteger(StackMachine* machine,__int64 arg)
{
	machine->getDataStack()->push(machine->getVariantFactory()->create<IntegerVariant,__int64>(Variant::INTEGER,arg));
	machine->advanceCounter();
}

__forceinline void pushString(StackMachine* machine,int staticIndex)
{
	StaticString* sString = static_cast<StaticString*>(machine->getStaticData(staticIndex));
	sString->getVariant()->addRef();
	machine->getDataStack()->push(sString->getVariant());
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


__forceinline void pushName(StackMachine* machine,int nameIndex)
{
	Variant* var = machine->getNearest(nameIndex);
	var->addRef();
	machine->getDataStack()->push(var);
	machine->advanceCounter();
}


__forceinline void pushFunction(StackMachine* machine,int address)
{
	machine->getDataStack()->push(new UserFunctionVariant(address));
	machine->advanceCounter();

}

__forceinline void pushNull(StackMachine* machine)
{
	machine->getDataStack()->pushNull();
	machine->advanceCounter();
}