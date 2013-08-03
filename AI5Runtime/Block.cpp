#include "Block.h"
#include "StackMachineThread.h"
#include "RuntimeError.h"

Block::Block(BLOCK_TYPE type):type(type)
{
	//this->stackRestorePoint = machine->getDataStack()->position();
}




void Block::unwindStack(StackMachineThread* machine,size_t stackPosition)
{
	DataStack* stack = machine->getDataStack();

	if(stack->size() < stackPosition)
	{
		throw RuntimeError(L"Stack corruption detected!");
	}

	// Pop off leftover stuff on the stack.
	// Note that this does not indicate that the block is
	// errorous, but rather that it quit in the middle
	// for some reason (perhaps exception or RET instruction).
	while(stack->size() > stackPosition)
	{
		stack->pop();
	}
}


bool Block::isCallBlock() const
{
	return type == Block::CALL_BLOCK;
}

bool Block::isLoopBlock() const
{
	return type == Block::LOOP_BLOCK;
}

bool Block::isCatchBlock() const
{
	return type == Block::CATCH_BLOCK;
}

bool Block::isGeneralBlock() const
{
	return type == Block::GENERAL_BLOCK;
}

bool Block::isFinallyBlock() const
{
	return type == Block::FINALLY_BLOCK;
}

BLOCK_TYPE Block::getType() const
{
	return type;
}