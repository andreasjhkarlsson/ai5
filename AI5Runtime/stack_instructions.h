#include "StackMachine.h"
#include "Integer64Variant.h"
#include "UserFunctionVariant.h"
#include "ListVariant.h"
#include "RuntimeError.h"
#include "LoopBlock.h"
#include "DefaultVariant.h"
#include "GeneralBlock.h"
#include "NameReferenceVariant.h"

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

__forceinline void doubleTopTwo(StackMachine* machine)
{
	DataStack *stack = machine->getDataStack();
	

	Variant* v1 = stack->get(0); // Same as top.
	Variant* v2 = stack->get(1);

	v2->addRef();
	v1->addRef();
	stack->push(v2);
	stack->push(v1);

	machine->advanceCounter();
}


__forceinline void pushNameValue(StackMachine* machine,NameIdentifier nameId)
{
	NameVariant* name = machine->getNearestName(nameId);

	if (name == nullptr)
	{
		StaticName* staticString = (StaticName*)machine->getStaticData(nameId.staticId);
		throw RuntimeError(std::wstring(L"Undeclared identifier ")+(*staticString->getName())+L"!");
	}


	Variant* var = name->getValue();
	var->addRef();
	machine->getDataStack()->push(var);
	machine->advanceCounter();
}



__forceinline void pushName(StackMachine* machine,NameIdentifier nameId)
{
	NameVariant* name = machine->getNearestName(nameId);

	if (name == nullptr)
	{
		StaticName* staticString = (StaticName*)machine->getStaticData(nameId.staticId);
		throw RuntimeError(std::wstring(L"Undeclared identifier ")+(*staticString->getName())+L"!");
	}

	name->addRef();

	machine->getDataStack()->push(name);
	machine->advanceCounter();
}

__forceinline void pushFunction(StackMachine* machine,int address)
{
	Scope *enclosingScope = nullptr;
	if(machine->getCurrentCallFrame() != nullptr)
		enclosingScope = machine->getCurrentCallFrame()->getScope();
	machine->getDataStack()->push(new UserFunctionVariant(address,enclosingScope));
	machine->advanceCounter();

}

__forceinline void pushNull(StackMachine* machine)
{
	machine->getDataStack()->pushNull();
	machine->advanceCounter();
}


__forceinline void pushDefault(StackMachine* machine)
{
	DefaultVariant* variant = &DefaultVariant::Instance;
	variant->addRef();
	machine->getDataStack()->push(variant);
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
	Variant* list = machine->getDataStack()->pop();

	if(!list->isListType())
	{
		throw RuntimeError(L"List indexing must have list type");
	}

	Variant* result = static_cast<ListVariant*>(list)->getElement(index->toInteger32());

	result->addRef();
	index->release();
	list->release();

	machine->getDataStack()->push(result);

	machine->advanceCounter();
}


__forceinline void pushMacro(StackMachine* machine,int arg)
{
	machine->getDataStack()->push(machine->getMacro(arg)(machine));
	machine->advanceCounter();
}


__forceinline void pushLoopBlock(StackMachine* machine,int continueAddress,int exitAddress)
{
	LoopBlock *lBlock = LoopBlock::getInstance();

	lBlock->setup(machine,continueAddress,exitAddress);

	machine->getBlockStack()->push(lBlock);

	machine->advanceCounter();

}

__forceinline void popBlock(StackMachine* machine)
{
	machine->getBlockStack()->top()->leave(machine);
	machine->getBlockStack()->pop()->recycleInstance();
	machine->advanceCounter();
}



__forceinline void pushGeneralBlock(StackMachine* machine)
{
	GeneralBlock* block = GeneralBlock::getInstance();
	block->setup(machine);
	
	machine->getBlockStack()->push(block);

	machine->advanceCounter();
}

__forceinline void swapTop(StackMachine* machine)
{
	Variant* v1 = machine->getDataStack()->pop();
	Variant* v2 = machine->getDataStack()->pop();
	machine->getDataStack()->push(v1);
	machine->getDataStack()->push(v2);
	machine->advanceCounter();
}