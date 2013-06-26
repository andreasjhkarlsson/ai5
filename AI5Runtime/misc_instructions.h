#include <stack>
#include "StackMachine.h"
#include "NullVariant.h"
#include "ListVariant.h"
#include "HashMapVariant.h"
#include "BooleanVariant.h"

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



__forceinline void assignNearest(StackMachine* machine,NameIdentifier arg)
{
	Variant* var = machine->getDataStack()->pop();
	
	machine->setNearest(arg,var);

	var->release();
	machine->advanceCounter();
}

__forceinline void makeGlobalConst(StackMachine* machine,NameIdentifier arg)
{
	machine->getGlobalName(arg)->markAsConst();
	machine->advanceCounter();
}

__forceinline void makeLocalConst(StackMachine* machine,NameIdentifier arg)
{
	machine->getLocalName(arg)->markAsConst();
	machine->advanceCounter();
}

__forceinline void makeNearestConst(StackMachine* machine,NameIdentifier arg)
{
	machine->getNearestName(arg)->markAsConst();
	machine->advanceCounter();
}


__forceinline void assignIndex(StackMachine* machine)
{
	Variant* value = machine->getDataStack()->pop();
	Variant* index = machine->getDataStack()->pop();
	Variant* container = machine->getDataStack()->pop();

	if(container->isListType())
	{
		static_cast<ListVariant*>(container)->setElement(index->toInteger32(),value);
	}
	else if(container->isHashMap())
	{
		static_cast<HashMapVariant*>(container)->set(index,value);
	}
	else
	{
		throw RuntimeError(L"List index assignment must have list type");
	}

	
	
	value->release();
	index->release();
	container->release();

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


ListVariant* createList(std::stack<unsigned int> subscripts);
void redimList(Variant* list,std::stack<unsigned int> subscripts);

__forceinline void createMultiDimList(StackMachine* machine,int numberOfSubscripts)
{
	// Use std::stack instead of FastStack since we need the copying mechanism
	// of std::stack which FastStack cannot handle (and shouldn't?).
	std::stack<unsigned int> subscripts;

	// Popping these arguments into another stack will reverse order (perfect!).
	for(int i=0;i<numberOfSubscripts;i++)
	{
		subscripts.push(machine->getDataStack()->top()->toInteger32());
		machine->getDataStack()->pop()->release();
	}

	ListVariant* resultList = createList(subscripts);

	machine->getDataStack()->push(resultList);

	machine->advanceCounter();
}


__forceinline void RedimMultiDimList(StackMachine* machine,int numberOfSubscripts)
{
	// Use std::stack instead of FastStack since we need the copying mechanism
	// of std::stack which FastStack cannot handle (and shouldn't?).
	std::stack<unsigned int> subscripts;

	// Popping these arguments into another stack will reverse order (perfect!).
	for(int i=0;i<numberOfSubscripts;i++)
	{
		subscripts.push((unsigned int)machine->getDataStack()->top()->toInteger32());
		machine->getDataStack()->pop()->release();
	}

	Variant* listVar = machine->getDataStack()->pop();


	redimList(listVar,subscripts);

	listVar->release();

	machine->advanceCounter();
}



inline void createClosureName(StackMachine* machine,NameIdentifier identifier)
{
	machine->getCurrentCallBlock()->addClosedName(machine,identifier);
	machine->advanceCounter();
}


inline void getIterator(StackMachine* machine)
{
	Variant* arg = machine->getDataStack()->pop();
	IteratorVariant* iterator = arg->iterate();
	machine->getDataStack()->push(iterator);
	arg->release();
	machine->advanceCounter();
}


inline void iteratorHasMore(StackMachine* machine)
{
	Variant* iter = machine->getDataStack()->pop();

	if(!iter->isIterator())
		throw RuntimeError(L"Top of stack is not iterator. This is most likely a compiler bug.");

	machine->getDataStack()->push(BooleanVariant::Get(static_cast<IteratorVariant*>(iter)->hasMore(),true));
	machine->advanceCounter();
}

inline void iteratorNext(StackMachine* machine)
{
	Variant* var = machine->getDataStack()->pop();

	if(!var->isIterator())
		throw RuntimeError(L"Top of stack is not iterator. This is most likely a compiler bug.");

	IteratorVariant* iter = static_cast<IteratorVariant*>(var);

	Variant* next = iter->next();
	next->addRef();
	machine->getDataStack()->push(next);
	machine->advanceCounter();

}