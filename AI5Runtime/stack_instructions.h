#include "StackMachine.h"
#include "Integer64Variant.h"
#include "UserFunctionVariant.h"
#include "ListVariant.h"



__forceinline void pushInteger64(StackMachine* machine,int arg)
{
	Integer64Variant* variant = static_cast<StaticInteger64*>(machine->getStaticData(arg))->getVariant();
	variant->addRef();
	machine->getDataStack()->push(variant);
	machine->advanceCounter();
}

__forceinline void pushInteger32(StackMachine* machine,int arg)
{
	Integer32Variant* variant = static_cast<StaticInteger32*>(machine->getStaticData(arg))->getVariant();
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


__forceinline void pushNameValue(StackMachine* machine,NameIdentifier nameId)
{
	Variant* var = machine->getNearestName(nameId)->getValue();
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

__forceinline void buildList(StackMachine* machine,int count)
{

	ListVariant* list = new ListVariant();

	for(int i=count-1;i>=0;i--)
	{
		Variant* element = machine->getDataStack()->get(i);
		list->addElement(element);
		element->release();
	}

	machine->getDataStack()->popMany(count);

	machine->getDataStack()->push(list);

	machine->advanceCounter();
}

__forceinline void derefIndex(StackMachine* machine)
{
	Variant* index = machine->getDataStack()->pop();
	ListVariant* list = static_cast<ListVariant*>(machine->getDataStack()->pop());

	Variant* result = list->getElement(index->toInteger32());

	result->addRef();
	index->release();
	list->release();

	machine->getDataStack()->push(result);

	machine->advanceCounter();
}


__forceinline void pushMacro(StackMachine* machine,int arg)
{
	machine->getDataStack()->push(machine->getMacro(arg)());
	machine->advanceCounter();
}