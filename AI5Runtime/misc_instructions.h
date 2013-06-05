#include <stack>
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

__forceinline void assignNearestConst(StackMachine* machine,NameIdentifier arg)
{
	Variant* var = machine->getDataStack()->pop();
	
	machine->setNearest(arg,var,true);

	var->release();
	machine->advanceCounter();
}

__forceinline void assignLocalConst(StackMachine* machine,NameIdentifier arg)
{
	Variant* var = machine->getDataStack()->pop();
	
	machine->setLocal(arg,var,true);

	var->release();
	machine->advanceCounter();
}

__forceinline void assignGlobalConst(StackMachine* machine,NameIdentifier arg)
{
	Variant* var = machine->getDataStack()->pop();
	
	machine->setGlobal(arg,var,true);

	var->release();
	machine->advanceCounter();
}


__forceinline void assignIndex(StackMachine* machine)
{
	Variant* value = machine->getDataStack()->pop();
	Variant* index = machine->getDataStack()->pop();
	Variant* list = machine->getDataStack()->pop();

	if(!list->isListType())
	{
		throw RuntimeError(L"List index assignment must have list type");
	}

	static_cast<ListVariant*>(list)->setElement(index->toInteger32(),value);
	
	value->release();
	index->release();
	list->release();

	machine->advanceCounter();
}

__forceinline void loadArgument(StackMachine* machine, NameIdentifier arg,bool asConst)
{
	// This is exactly the same thing.
	if(asConst)
		assignLocalConst(machine,arg);
	else
		assignLocal(machine,arg);
}

__forceinline void loadByRefArgument(StackMachine* machine,NameIdentifier arg,bool asConst)
{

	if(machine->getDataStack()->top()->getLastName() != nullptr)
	{
		Variant* var = machine->getDataStack()->pop();

		if(asConst && var->getLastName()->isConstName())
		{

			// What to do with this situation??
			// If we mark the name as const we will make the source name
			// const as well... Maybe add nest the names?
			

		}

		machine->addNameToLocalScope(arg,var->getLastName());
		machine->advanceCounter();
	}
	else
	{
		loadArgument(machine,arg,asConst);
	}

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


// Inlining recursive function?? Crazy shit!!
__forceinline ListVariant* createList(std::stack<int> subscripts)
{

	int count = subscripts.top();
	subscripts.pop();

	ListVariant* list = new ListVariant();
	if(subscripts.size() == 0)
	{
		for(int i=0;i<count;i++)
			list->addElement(&NullVariant::Instance);
		return list;
	}

	for(int i=0;i<count;i++)
	{
		list->addElement(createList(subscripts));
	}

	return list;

}

__forceinline void createMultiDimList(StackMachine* machine,int numberOfSubscripts)
{

	
	std::stack<int> subscripts;


	for(int i=0;i<numberOfSubscripts;i++)
	{
		subscripts.push(machine->getDataStack()->top()->toInteger32());
		machine->getDataStack()->pop()->release();
	}

	// The subscript sizes where popped in reverse order.
	//std::reverse(subscripts.begin(),subscripts.end());

	ListVariant* resultList = createList(subscripts);

	machine->getDataStack()->push(resultList);


	machine->advanceCounter();

}