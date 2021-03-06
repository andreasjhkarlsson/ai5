#pragma once
#include <stack>
#include "StackMachineThread.h"
#include "ListVariant.h"
#include "HashMapVariant.h"
#include "BooleanVariant.h"
#include "gc.h"
#include "function_instructions.h"
#include "CatchBlock.h"
#include "FinallyBlock.h"
#include "jump_instructions.h"

__forceinline void noop(StackMachineThread* machine)
{
	machine->advanceCounter();
}

__forceinline void terminate(StackMachineThread* machine)
{
	machine->terminate(machine->getDataStack()->pop().toInteger32());
}

__forceinline void assignGlobal(StackMachineThread* machine, NameIdentifier arg)
{
	VariantReference<> var = machine->getDataStack()->pop();
	
	machine->setGlobal(arg,var);

	machine->advanceCounter();
}

__forceinline void assignLocal(StackMachineThread* machine, NameIdentifier arg)
{
	VariantReference<> var = machine->getDataStack()->pop();
	
	machine->setLocal(arg,var);

	machine->advanceCounter();
}



__forceinline void assignNearest(StackMachineThread* machine,NameIdentifier arg)
{
	VariantReference<> var = machine->getDataStack()->pop();
	
	machine->setNearest(arg,var);

	machine->advanceCounter();
}

__forceinline void makeGlobalConst(StackMachineThread* machine,NameIdentifier arg)
{
	machine->getGlobalName(arg).cast<NameVariant>()->markAsConst();
	machine->advanceCounter();
}

__forceinline void makeLocalConst(StackMachineThread* machine,NameIdentifier arg)
{
	machine->getLocalName(arg).cast<NameVariant>()->markAsConst();
	machine->advanceCounter();
}

__forceinline void makeNearestConst(StackMachineThread* machine,NameIdentifier arg)
{
	machine->getNearestName(arg).cast<NameVariant>()->markAsConst();
	machine->advanceCounter();
}


__forceinline void assignIndex(StackMachineThread* machine)
{
	VariantReference<> value = machine->getDataStack()->pop();
	VariantReference<> index = machine->getDataStack()->pop();
	VariantReference<> container = machine->getDataStack()->pop();

	if(container.isListType())
	{
		container.cast<ListVariant>()->setElement(index.toInteger32(),value);
	}
	else if(container.isHashMap())
	{
		container.cast<HashMapVariant>()->set(index,value);
	}
	else
	{
		throw RuntimeError(L"List index assignment must have list type");
	}

	machine->advanceCounter();
}


__forceinline void concatStrings(StackMachineThread* machine)
{

	VariantReference<> arg2 = machine->getDataStack()->pop();
	VariantReference<> arg1 = machine->getDataStack()->pop();

	shared_string result = shared_string(new UnicodeString(L""));
	(*result)+=*arg1.toString();
	(*result)+=*arg2.toString();


	machine->getDataStack()->push(StringVariant::Create(result));

	machine->advanceCounter();
}


ListVariant* createList(std::stack<unsigned int> subscripts);
void redimList(VariantReference<ListVariant>& list,std::stack<unsigned int> subscripts);

__forceinline void createMultiDimList(StackMachineThread* machine,int numberOfSubscripts)
{
	// Use std::stack instead of FastStack since we need the copying mechanism
	// of std::stack which FastStack cannot handle (and shouldn't?).
	std::stack<unsigned int> subscripts;

	// Popping these arguments into another stack will reverse order (perfect!).
	for(int i=0;i<numberOfSubscripts;i++)
	{
		subscripts.push(machine->getDataStack()->top().toInteger32());
		machine->getDataStack()->pop();
	}

	machine->getDataStack()->push(createList(subscripts));

	machine->advanceCounter();
}


__forceinline void RedimMultiDimList(StackMachineThread* machine,int numberOfSubscripts)
{
	// Use std::stack instead of FastStack since we need the copying mechanism
	// of std::stack which FastStack cannot handle (and shouldn't?).
	std::stack<unsigned int> subscripts;

	// Popping these arguments into another stack will reverse order (perfect!).
	for(int i=0;i<numberOfSubscripts;i++)
	{
		subscripts.push((unsigned int)machine->getDataStack()->top().toInteger32());
		machine->getDataStack()->pop();
	}

	VariantReference<ListVariant> listVar = machine->getDataStack()->pop().cast<ListVariant>();

	redimList(listVar,subscripts);

	machine->advanceCounter();
}



inline void createClosureName(StackMachineThread* machine,NameIdentifier identifier)
{
	machine->getCurrentCallBlock()->addClosedName(machine,identifier);
	machine->advanceCounter();
}


inline void getIterator(StackMachineThread* machine)
{
	VariantReference<IteratorVariant> arg = machine->getDataStack()->pop()->iterate();
	machine->getDataStack()->push(arg);
	machine->advanceCounter();
}


inline void iteratorHasMore(StackMachineThread* machine)
{
	VariantReference<IteratorVariant> iter = machine->getDataStack()->pop().cast<IteratorVariant>();

	machine->getDataStack()->push(iter->hasMore());
	machine->advanceCounter();
}

inline void iteratorNext(StackMachineThread* machine)
{
	VariantReference<IteratorVariant> iter = machine->getDataStack()->pop().cast<IteratorVariant>();
	VariantReference<> val = iter->next();
	machine->getDataStack()->push(val);
	machine->advanceCounter();

}

inline void throwException(StackMachineThread* machine)
{
	machine->setCurrentException(machine->getDataStack()->pop());
	
	BlockStack* bStack = machine->getBlockStack();

	while(true)
	{

		if(bStack->empty())
		{
			// No catch handler found for exception!! 
			throw RuntimeError(UnicodeString(L"Uncaught exception: ")+(*machine->getCurrentException()->toString()));
		}

		
		if(bStack->top()->isCatchBlock())
			break;

		Block* block = bStack->pop();

		if(block->isFinallyBlock())
		{
			static_cast<FinallyBlock*>(block)->setRethrowAction(machine->getCurrentException());
			static_cast<FinallyBlock*>(block)->execute(machine);
			return;
		}

		if(block->isCallBlock())
		{
			machine->setCurrentCallBlock(static_cast<CallBlock*>(block)->getParentFrame());

		}
		block->leave(machine);
		block->recycleInstance();

	}

	machine->jumpAbsolute(static_cast<CatchBlock*>(bStack->pop())->getAddress());
}

inline void pushCurrentException(StackMachineThread* machine)
{
	machine->getDataStack()->push(machine->getCurrentException());
	machine->advanceCounter();
}

inline void exitFinallyBlock(StackMachineThread* machine)
{
	FinallyBlock* block = static_cast<FinallyBlock*>(machine->getBlockStack()->pop());
	const FinallyBlock::ReturnInfo& returnInfo = block->getReturnInfo();
	block->leave(machine);
	switch(returnInfo.action)
	{
	case FinallyBlock::ReturnInfo::JUMP_TO:
		machine->jumpAbsolute(returnInfo.returnAddress);
		break;
	case FinallyBlock::ReturnInfo::THROW_EXCEPTION:
		machine->getDataStack()->push(returnInfo.exceptionToThrow);
		throwException(machine);
		break;
	case FinallyBlock::ReturnInfo::RETURN_FROM_FUNC:
		machine->getDataStack()->push(returnInfo.returnValue);
		ret(machine);
		break;
	case FinallyBlock::ReturnInfo::EXITLOOP:
		loopJump(LOOP_JUMP_TYPE::BREAK,machine,returnInfo.level);
		break;
	case FinallyBlock::ReturnInfo::CONTINUELOOP:
		loopJump(LOOP_JUMP_TYPE::CONTINUE,machine,returnInfo.level);
		break;
	}
	block->recycleInstance();
}