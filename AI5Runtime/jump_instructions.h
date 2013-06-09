#include "StackMachine.h"
#include "UserFunctionVariant.h"

__forceinline void jumpLongRelative(StackMachine* machine,unsigned int arg)
{
	machine->jumpRelative(arg);
}

__forceinline void jumpLongRelativeIfFalse(StackMachine* machine,unsigned int arg)
{
	Variant *v = machine->getDataStack()->pop();

	bool cond;
	if(v->getType() == Variant::BOOLEAN)
		cond = ((BooleanVariant*)v)->value;
	else
		cond = v->toBoolean();

	if(!cond)
		machine->jumpRelative(arg);
	else
		machine->advanceCounter();
	v->release();
}

__forceinline void jumpShortRelativeIfFalse(StackMachine* machine,char arg)
{
	jumpLongRelativeIfFalse(machine,arg);
}

__forceinline void jumpShortRelative(StackMachine* machine,char arg)
{
	jumpLongRelative(machine,arg);
}

__forceinline void jumpLongRelativeIfTrue(StackMachine* machine,unsigned int arg)
{
	Variant *v = machine->getDataStack()->pop();

	bool cond;
	if(v->getType() == Variant::BOOLEAN)
		cond = ((BooleanVariant*)v)->value;
	else
		cond = v->toBoolean();

	if(cond)
		machine->jumpRelative(arg);
	else
		machine->advanceCounter();
	v->release();
}


enum LOOP_JUMP_TYPE
{
	CONTINUE,
	BREAK
};

inline void loopJump(LOOP_JUMP_TYPE type,StackMachine* machine,int level)
{

	BlockStack* stack = machine->getBlockStack();

	// Pop all the blocks until encountering loop block at the correct level.
	while(true)
	{
		Block* block = stack->top();

		if(block->isCallBlock())
		{
			throw RuntimeError(L"Invalid use of continue/exitloop");
		}

		// Some other block. leave it!
		if(!block->isLoopBlock() || --level > 0)
		{
			block->leave(machine);
			block->recycleInstance();
			stack->pop();
			continue;
		}

		if(type == CONTINUE)
		{
			machine->jumpAbsolute(static_cast<LoopBlock*>(block)->getContinueAddress());
		}
		else if( type == BREAK)
		{
			stack->pop();
			block->leave(machine);
			block->recycleInstance();
			machine->jumpAbsolute(static_cast<LoopBlock*>(block)->getExitAddress());
		}

		break;
	}
}



