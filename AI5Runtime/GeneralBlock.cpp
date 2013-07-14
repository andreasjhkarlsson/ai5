#include "GeneralBlock.h"
#include "StackMachineThread.h"

GeneralBlock::GeneralBlock(void): Block(GENERAL_BLOCK)
{
}


GeneralBlock::~GeneralBlock(void)
{
}

void GeneralBlock::leave(StackMachineThread* machine)
{
	Block::unwindStack(machine,stackPos);
}
void GeneralBlock::setup(StackMachineThread* machine)
{
	this->stackPos = machine->getDataStack()->size();
}
void GeneralBlock::recycleInstance()
{
	GeneralBlock::returnInstance(this);
}