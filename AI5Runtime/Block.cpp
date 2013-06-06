#include "Block.h"
#include "StackMachine.h"
#include "RuntimeError.h"

Block::Block(int stackRestorePosition):stackRestorePoint(stackRestorePosition)
{
	//this->stackRestorePoint = machine->getDataStack()->position();
}

void Block::leave(StackMachine* machine)
{
	DataStack* stack = machine->getDataStack();

	if(stack->position() < stackRestorePoint)
	{
		throw RuntimeError(L"Stack corruption detected!");
	}

	// Pop off leftover stuff on the stack.
	// Note that this does not indicate that the block is
	// errorous, but rather that it quit in the middle
	// for some reason (perhaps exception or RET instruction).
	while(stack->position() > stackRestorePoint)
	{
		stack->pop()->release();
	}

}