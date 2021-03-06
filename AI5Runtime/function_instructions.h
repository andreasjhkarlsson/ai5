#pragma once
#include "Block.h"
#include "Variant.h"
#include "StackMachineThread.h"
#include "UserFunctionVariant.h"
#include "CallBlock.h"
#include "FinallyBlock.h"

__forceinline void callFunction(StackMachineThread* machine,unsigned int numberOfArgs)
{
	// The actual object resides below the arguments on the stack. 
	// It's up the the function to pop arguments + function from stack.
	VariantReference<> toCall = machine->getDataStack()->get(numberOfArgs);

	if(toCall.isUserFunctionType())
	{
		VariantReference<UserFunctionVariant> userFunc = toCall.cast<UserFunctionVariant>();
		int address = userFunc->getAddress();

		CallBlock* frame = CallBlock::getInstance();

		frame->setup(machine,machine->getCurrentAddress()+1,numberOfArgs,machine->getCurrentCallBlock(),userFunc);

		machine->getBlockStack()->push(frame);


		machine->setCurrentCallBlock(frame);
		machine->jumpAbsolute(address);		
	}
	else if(toCall.isNativeFunctionType())
	{
		toCall.cast<BuiltinFunctionVariant>()->call(machine,numberOfArgs);
	}
	else
	{
		throw RuntimeError(L"Value not callable!");
	}
}

__forceinline void ret(StackMachineThread* machine)
{
	// Temporarily pop return value from stack.
	// This is needed because popping CallBlock will unwind
	// anything added to the stack during calling (so it will be clean during exceptions for example).
	// Don't worry, we'll add it back after the unwinding is done.
	VariantReference<> returnValue = machine->getDataStack()->pop();

	BlockStack* blockStack = machine->getBlockStack();

	while(!blockStack->top()->isCallBlock())
	{

		Block* block = blockStack->pop();

		if(block->isFinallyBlock())
		{
			FinallyBlock* finallyBlock = static_cast<FinallyBlock*>(block);
			finallyBlock->setFunctionReturnAction(returnValue);
			finallyBlock->execute(machine);
			return;
		}

		block->leave(machine);
		block->recycleInstance();
	}

	CallBlock* frame = static_cast<CallBlock*>(blockStack->pop());

	// Make sure stack is balanced (maybe throw corruption error here
	// since the stack should always be balanced when RET is executed).
	frame->leave(machine);
	machine->jumpAbsolute(frame->getReturnAddress());
	machine->setCurrentCallBlock(frame->getParentFrame());
	frame->recycleInstance();

	// I told you I would return it to the stack.
	machine->getDataStack()->push(returnValue);
}


__forceinline void createArgument(StackMachineThread* machine,NameIdentifier identifier,bool byref)
{
	CallBlock* frame = machine->getCurrentCallBlock();

	Argument arg;
	arg.identifier = identifier;
	arg.isByref = byref;

	frame->addArgument(arg);

	machine->advanceCounter();

}


__forceinline void loadArguments(StackMachineThread* machine,int total,int required)
{
	machine->getCurrentCallBlock()->loadArguments(machine,total,required);
	machine->advanceCounter();
}