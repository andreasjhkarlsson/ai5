#pragma once
#include "Block.h"
#include "Variant.h"
#include "StackMachine.h"
#include "UserFunctionVariant.h"
#include "CallFrame.h"

__forceinline void callFunction(StackMachine* machine,unsigned int numberOfArgs)
{
	// The actual object resides below the arguments on the stack. 
	// It's up the the function to pop arguments + function from stack.
	Variant* toCall = machine->getDataStack()->get(numberOfArgs);

	if(toCall->getType() == Variant::USER_FUNCTION)
	{
		int address = ((UserFunctionVariant*)toCall)->getAddress();

		CallFrame* frame = CallFrame::getInstance();

		frame->setup(machine,machine->getCurrentAddress()+1,numberOfArgs,machine->getCurrentCallFrame());

		machine->getBlockStack()->push(frame);


		machine->setCurrentCallFrame(frame);
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

	BlockStack* blockStack = machine->getBlockStack();

	while(!blockStack->top()->isCallBlock())
	{
		blockStack->top()->leave(machine);
		blockStack->pop()->recycleInstance();
	}

	CallFrame* frame = static_cast<CallFrame*>(blockStack->pop());

	// Make sure stack is balanced (maybe throw corruption error here
	// since the stack should always be balanced when RET is executed).
	frame->leave(machine);
	machine->jumpAbsolute(frame->getReturnAddress());
	machine->setCurrentCallFrame(frame->getParentFrame());
	frame->recycleInstance();

	// I told you I would return it to the stack.
	machine->getDataStack()->push(returnValue);
}


__forceinline void createArgument(StackMachine* machine,NameIdentifier identifier,bool byref)
{
	CallFrame* frame = machine->getCurrentCallFrame();

	Argument arg;
	arg.identifier = identifier;
	arg.isByref = byref;

	frame->addArgument(arg);

	machine->advanceCounter();

}


__forceinline void loadArguments(StackMachine* machine,int total,int required)
{
	machine->getCurrentCallFrame()->loadArguments(machine,total,required);
	machine->advanceCounter();
}