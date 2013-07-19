#include "StackMachineThread.h"
#include "Integer64Variant.h"
#include "UserFunctionVariant.h"
#include "ListVariant.h"
#include "RuntimeError.h"
#include "LoopBlock.h"
#include "DefaultVariant.h"
#include "GeneralBlock.h"
#include "NameReferenceVariant.h"
#include "HashMapVariant.h"

__forceinline void pushInteger64(StackMachineThread* machine,int arg)
{
	VariantReference<> variant = static_cast<StaticInteger64*>(machine->getStaticData(arg))->getInt();
	machine->getDataStack()->push(variant);
	machine->advanceCounter();
}

__forceinline void pushInteger32(StackMachineThread* machine,int arg)
{
	VariantReference<> variant = static_cast<StaticInteger32*>(machine->getStaticData(arg))->getInt();
	machine->getDataStack()->push(variant);
	machine->advanceCounter();
}

__forceinline void pushFloating(StackMachineThread* machine,int arg)
{
	VariantReference<> variant = static_cast<StaticFloating*>(machine->getStaticData(arg))->getFloating();
	machine->getDataStack()->push(variant);
	machine->advanceCounter();
}

__forceinline void pushString(StackMachineThread* machine,int staticIndex)
{
	StaticString* sString = static_cast<StaticString*>(machine->getStaticData(staticIndex));
	machine->getDataStack()->push(sString->getVariant());
	machine->advanceCounter();
}

__forceinline void pop(StackMachineThread* machine)
{
	machine->getDataStack()->pop();
	machine->advanceCounter();
}

__forceinline void doubleTop(StackMachineThread* machine)
{
	DataStack *stack = machine->getDataStack();
	stack->push(stack->top());
	machine->advanceCounter();
}

__forceinline void doubleTopTwo(StackMachineThread* machine)
{
	DataStack *stack = machine->getDataStack();
	

	VariantReference<> v1 = stack->get(0); // Same as top.
	VariantReference<> v2 = stack->get(1);

	stack->push(v2);
	stack->push(v1);

	machine->advanceCounter();
}

__forceinline void pushNameValue(StackMachineThread* machine,NameIdentifier nameId)
{
	VariantReference<NameVariant> name = machine->getNearestName(nameId);

	if (name.empty())
	{
		StaticName* staticString = (StaticName*)machine->getStaticData(nameId.staticId);
		throw RuntimeError(UnicodeString(L"Undeclared identifier ")+(*staticString->getName())+L"!");
	}

	const VariantReference<>& var = name->getValue();
	machine->getDataStack()->push(var);
	machine->advanceCounter();
}


__forceinline void pushName(StackMachineThread* machine,NameIdentifier nameId)
{
	VariantReference<NameVariant> name = machine->getNearestName(nameId);

	if (name.empty())
	{
		StaticName* staticString = (StaticName*)machine->getStaticData(nameId.staticId);
		throw RuntimeError(UnicodeString(L"Undeclared identifier ")+(*staticString->getName())+L"!");
	}

	machine->getDataStack()->push(name.cast<Variant>());
	machine->advanceCounter();
}

__forceinline void pushFunction(StackMachineThread* machine,int address)
{
	VariantReference<UserFunctionVariant> fn = UserFunctionVariant::Create(address);
	if(machine->getCurrentCallBlock() != nullptr)
	{
		machine->getCurrentCallBlock()->addClosure(machine,fn);
	}
	machine->getDataStack()->push(fn);
	machine->advanceCounter();

}

__forceinline void pushNull(StackMachineThread* machine)
{
	machine->getDataStack()->pushNull();
	machine->advanceCounter();
}

__forceinline void pushDefault(StackMachineThread* machine)
{
	machine->getDataStack()->push(VariantReference<>::DefaultReference());
	machine->advanceCounter();
}


__forceinline void pushBoolean(StackMachineThread* machine,char arg)
{
	machine->getDataStack()->push(arg != 0);
	machine->advanceCounter();
}

inline void buildList(StackMachineThread* machine,int count)
{

	ListVariant* list = ListVariant::Create();

	for(int i=count-1;i>=0;i--)
	{
		VariantReference<> element = machine->getDataStack()->get(i);
		list->addElement(element);
	}

	machine->getDataStack()->popMany(count);

	machine->getDataStack()->push(list);

	machine->advanceCounter();
}

inline void buildMap(StackMachineThread* machine,int count)
{
	VariantReference<HashMapVariant> map = HashMapVariant::Create();

	// Since a hash map is not sorted
	// it doesn't matter which order the arguments are popped
	for(int i=0;i<count;i++)
	{
		VariantReference<> value = machine->getDataStack()->pop();
		VariantReference<> key = machine->getDataStack()->pop();
		map->set(key,value);
	}

	machine->getDataStack()->push(map);
	machine->advanceCounter();
}

__forceinline void derefIndex(StackMachineThread* machine)
{
	VariantReference<> index = machine->getDataStack()->pop();
	VariantReference<> container = machine->getDataStack()->pop();
	VariantReference<> result = nullptr;
	if(container.isListType())
	{
		result = container.cast<ListVariant>()->getElement(index.toInteger32());
	}
	else if(container.isHashMap())
	{
		result = container.cast<HashMapVariant>()->get(index);
	}
	else
	{
		throw RuntimeError(L"List indexing must have list type");
	}

	machine->getDataStack()->push(result);

	machine->advanceCounter();
}

__forceinline void pushMacro(StackMachineThread* machine,int arg)
{
	machine->getDataStack()->push(machine->getMacro(arg)(machine));
	machine->advanceCounter();
}

__forceinline void pushLoopBlock(StackMachineThread* machine,int continueAddress,int exitAddress)
{
	LoopBlock *lBlock = LoopBlock::getInstance();

	lBlock->setup(machine,continueAddress,exitAddress);

	machine->getBlockStack()->push(lBlock);

	machine->advanceCounter();
}

__forceinline void popBlock(StackMachineThread* machine)
{
	machine->getBlockStack()->top()->leave(machine);
	machine->getBlockStack()->pop()->recycleInstance();
	machine->advanceCounter();
}

__forceinline void pushGeneralBlock(StackMachineThread* machine)
{
	GeneralBlock* block = GeneralBlock::getInstance();
	block->setup(machine);
	
	machine->getBlockStack()->push(block);

	machine->advanceCounter();
}

__forceinline void swapTop(StackMachineThread* machine)
{
	VariantReference<> v1 = machine->getDataStack()->pop();
	VariantReference<> v2 = machine->getDataStack()->pop();
	machine->getDataStack()->push(v1);
	machine->getDataStack()->push(v2);
	machine->advanceCounter();
}