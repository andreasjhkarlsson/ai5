#include "CallFrame.h"
#include "StackMachine.h"

CallFrame::CallFrame(): Block(0)
{
}

CallFrame::~CallFrame(void)
{
}


void CallFrame::setup(StackMachine* machine,int numberOfArguments)
{
	this->returnAddress = machine->getCurrentAddress()+1;
	this->setStackReturnPoint(machine->getDataStack()->position()-(numberOfArguments+1));
}

void CallFrame::leave(StackMachine*machine)
{
	Block::leave(machine);

	scope.reset();

	machine->jumpAbsolute(returnAddress);
}

Scope* CallFrame::getScope()
{
	return &scope;
}