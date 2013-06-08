#include "Block.h"
#include "StackMachine.h"
#include "RuntimeError.h"

Block::Block(BLOCK_TYPE type):type(type)
{
	//this->stackRestorePoint = machine->getDataStack()->position();
}




void Block::unwindStack(StackMachine* machine,int stackPosition)
{
	DataStack* stack = machine->getDataStack();

	if(stack->position() < stackPosition)
	{
		throw RuntimeError(L"Stack corruption detected!");
	}

	// Pop off leftover stuff on the stack.
	// Note that this does not indicate that the block is
	// errorous, but rather that it quit in the middle
	// for some reason (perhaps exception or RET instruction).
	while(stack->position() > stackPosition)
	{
		stack->pop()->release();
	}
}