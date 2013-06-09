#include "CallFrame.h"
#include "StackMachine.h"

CallFrame::CallFrame(): Block(CALL_BLOCK)
{
}

CallFrame::~CallFrame(void)
{
}


void CallFrame::setup(StackMachine* machine,int returnAddress,int numberOfArguments,CallFrame* parentFrame)
{
	this->returnAddress = returnAddress;
	this->stackPosition = machine->getDataStack()->position()-(numberOfArguments+1);
	this->numberOfArguments = numberOfArguments;
	this->parentFrame = parentFrame;
}

void CallFrame::leave(StackMachine*machine)
{
	unwindStack(machine,stackPosition);

	scope.reset();
}

Scope* CallFrame::getScope()
{
	return &scope;
}


void CallFrame::recycleInstance()
{
	CallFrame::returnInstance(this);
}

int CallFrame::getReturnAddress()
{
	return returnAddress;

}

CallFrame* CallFrame::getParentFrame()
{
	return this->parentFrame;
}