#include "LoopBlock.h"
#include "StackMachineThread.h"

LoopBlock::LoopBlock(void): Block(LOOP_BLOCK)
{
}


LoopBlock::~LoopBlock(void)
{
}


void LoopBlock::recycleInstance()
{
	LoopBlock::returnInstance(this);
}


void LoopBlock::leave(StackMachineThread* machine)
{
	Block::unwindStack(machine,stackPosition);
}

void LoopBlock::setup(StackMachineThread* machine,int continuePosition,int exitPosition)
{
	this->stackPosition = machine->getDataStack()->size();
	this->exitPosition = exitPosition;
	this->continuePosition = continuePosition;
}


int LoopBlock::getContinueAddress()
{
	return continuePosition;
}

int LoopBlock::getExitAddress()
{
	return exitPosition;
}