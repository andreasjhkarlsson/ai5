#include "CallFrame.h"
#include "StackMachine.h"

CallFrame::CallFrame(): Block(CALL_BLOCK)
{
}

CallFrame::~CallFrame(void)
{
}


void CallFrame::setup(StackMachine* machine,int returnAddress,int numberOfArguments)
{
	this->returnAddress = returnAddress;
	this->stackPosition = machine->getDataStack()->position()-(numberOfArguments+1);
}

void CallFrame::leave(StackMachine*machine)
{
	unwindStack(machine,stackPosition);

	scope.reset();

	machine->jumpAbsolute(returnAddress);
}

Scope* CallFrame::getScope()
{
	return &scope;
}