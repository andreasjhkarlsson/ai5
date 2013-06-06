#include "CallFrame.h"
#include "StackMachine.h"

CallFrame::CallFrame(StackMachine* machine,int numberOfArguments): Block(machine->getDataStack()->position()-(numberOfArguments+1)),
	returnAddress(machine->getCurrentAddress()+1),numberOfArguments(numberOfArguments)
{
}

CallFrame::~CallFrame(void)
{
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