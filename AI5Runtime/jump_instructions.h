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

__forceinline void callFunction(StackMachine* machine,unsigned int numberOfArgs)
{
	// The actual object resides below the arguments on the stack. 
	// It's up the the function to pop arguments + function from stack.
	Variant* toCall = machine->getDataStack()->get(numberOfArgs);

	if(toCall->getType() == Variant::USER_FUNCTION)
	{
		int address = ((UserFunctionVariant*)toCall)->getAddress();

		CallFrame* frame = CallFrame::getInstance();

		frame->setup(machine,machine->getCurrentAddress()+1,numberOfArgs);

		machine->pushBlock(frame);

		machine->jumpAbsolute(address);		
	}
	else if(toCall->getType() == Variant::NATIVE_FUNCTION)
	{
		((BuiltinFunctionVariant*)toCall)->call(machine,numberOfArgs);
	}
	else
	{
		throw RuntimeError(L"Value not callable!");
	}
}

__forceinline void ret(StackMachine* machine)
{
	// Temporarily pop return value from stack.
	// This is needed because popping callframe will unwind
	// anything added to the stack during calling (so it will be clean during exceptions for example9.
	// Don't worry, we'll add it back after the unwinding is done.
	Variant* returnValue = machine->getDataStack()->pop();

	// This fixes funky behaviour with byref. 
	// For example, the following code:
	//--------------
	// Global a = 10
	// Func foo()
	//   return a
	// EndFunc
	// Func bar(ByRef b)
	//   b *= 2
	// EndFunc
	// bar(foo())
	// printline(a)
	// --------------
	// Will print '20' instead of the more sensible '10'.
	returnValue->setLastName(nullptr);

	while(!machine->topBlock()->isCallBlock())
	{
		machine->topBlock()->leave(machine);
		machine->popBlock()->recycleInstance();
	}

	CallFrame* frame = static_cast<CallFrame*>(machine->popBlock());
	frame->leave(machine);
	int returnAddress = frame->getReturnAddress();
	frame->recycleInstance();

	// I told you I would return it to the stack.
	machine->getDataStack()->push(returnValue);

	machine->jumpAbsolute(returnAddress);
}


enum LOOP_JUMP_TYPE
{
	CONTINUE,
	BREAK
};

inline void loopJump(LOOP_JUMP_TYPE type,StackMachine* machine,int level)
{
	while(true)
	{
		Block* block = machine->topBlock();

		// Some other block. skip it!
		if(!block->isLoopBlock() || --level > 0)
		{
			block->leave(machine);
			block->recycleInstance();
			machine->popBlock();
			continue;
		}

		if(type == CONTINUE)
		{
			machine->jumpAbsolute(static_cast<LoopBlock*>(block)->getContinueAddress());
		}
		else if( type == BREAK)
		{
			machine->popBlock();
			block->leave(machine);
			block->recycleInstance();
			machine->jumpAbsolute(static_cast<LoopBlock*>(block)->getExitAddress());
		}

		break;
	}
}



