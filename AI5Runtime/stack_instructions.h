#include "StackMachine.h"
#include "IntegerVariant.h"
#include "UserFunctionVariant.h"
#include "ListVariant.h"



__forceinline void pushInteger(StackMachine* machine,int arg)
{
	IntegerVariant* variant = static_cast<StaticInteger*>(machine->getStaticData(arg))->getVariant();
	variant->addRef();
	machine->getDataStack()->push(variant);
	machine->advanceCounter();
}

__forceinline void pushFloating(StackMachine* machine,int arg)
{
	FloatingVariant* variant = static_cast<StaticFloating*>(machine->getStaticData(arg))->getVariant();
	variant->addRef();
	machine->getDataStack()->push(variant);
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


__forceinline void pushName(StackMachine* machine,NameIdentifier nameId)
{
	Variant* var = machine->getNearest(nameId);
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

__forceinline void pushBoolean(StackMachine* machine,char arg)
{
	Variant* var = nullptr;

	if(arg)
		var = &BooleanVariant::True;
	else
		var = &BooleanVariant::False;

	var->addRef();
	machine->getDataStack()->push(var);
	machine->advanceCounter();
}

__forceinline void pushEmptyList(StackMachine* machine)
{
	machine->getDataStack()->push(new ListVariant());
	machine->advanceCounter();
}

__forceinline void addListElement(StackMachine* machine)
{
	// No need to release element as we would have
	// addRef'ed it anyway when adding it to the list.
	Variant* element = machine->getDataStack()->pop();

	// No need to pop list as it should be left on the stack when the instruction returns.
	ListVariant* list = static_cast<ListVariant*>(machine->getDataStack()->top());

	list->addElement(element);

	element->release();

	
	machine->advanceCounter();
}