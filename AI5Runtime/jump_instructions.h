#include "StackMachineThread.h"
#include "UserFunctionVariant.h"

__forceinline void jumpLongRelative(StackMachineThread* machine,unsigned int arg)
{
	machine->jumpRelative(arg);
}

__forceinline void jumpLongRelativeIfFalse(StackMachineThread* machine,unsigned int arg)
{
	VariantReference<> v = machine->getDataStack()->pop();

	if(!v.toBoolean())
		machine->jumpRelative(arg);
	else
		machine->advanceCounter();
}

__forceinline void jumpShortRelativeIfFalse(StackMachineThread* machine,char arg)
{
	jumpLongRelativeIfFalse(machine,arg);
}

__forceinline void jumpLongAbsoluteIfFalse(StackMachineThread* machine,unsigned int arg)
{
	VariantReference<> v = machine->getDataStack()->pop();

	if(!v.toBoolean())
		machine->jumpAbsolute(arg);
	else
		machine->advanceCounter();
}

__forceinline void jumpShortAbsoluteIfFalse(StackMachineThread* machine,char arg)
{
	jumpLongAbsoluteIfFalse(machine,arg);
}

__forceinline void jumpShortRelative(StackMachineThread* machine,char arg)
{
	jumpLongRelative(machine,arg);
}

__forceinline void jumpLongRelativeIfTrue(StackMachineThread* machine,unsigned int arg)
{
	VariantReference<> v = machine->getDataStack()->pop();

	if(v.toBoolean())
		machine->jumpRelative(arg);
	else
		machine->advanceCounter();

}

__forceinline void jumpLongAbsolute(StackMachineThread* machine,unsigned int arg)
{	
	machine->jumpAbsolute(arg);
}

__forceinline void jumpShortAbsolute(StackMachineThread* machine,char arg)
{
	jumpLongAbsolute(machine,arg);
}

enum LOOP_JUMP_TYPE
{
	CONTINUE,
	BREAK
};

inline void loopJump(LOOP_JUMP_TYPE type,StackMachineThread* machine,int level)
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



