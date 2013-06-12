#include "GeneralBlock.h"
#include "StackMachine.h"

GeneralBlock::GeneralBlock(void): Block(GENERAL_BLOCK)
{
}


GeneralBlock::~GeneralBlock(void)
{
}

void GeneralBlock::leave(StackMachine* machine)
{
	Block::unwindStack(machine,stackPos);
}
void GeneralBlock::setup(StackMachine* machine)
{
	this->stackPos = machine->getDataStack()->size();
}
void GeneralBlock::recycleInstance()
{
	GeneralBlock::returnInstance(this);
}