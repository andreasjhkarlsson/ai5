#include "CatchBlock.h"
#include "StackMachineThread.h"

CatchBlock::CatchBlock(): Block(CATCH_BLOCK)
{
}

CatchBlock::~CatchBlock(void)
{
}

void CatchBlock::setup(StackMachineThread* machine,int address)
{
	this->address = address;
	this->stackPos = machine->getDataStack()->size();
}

void CatchBlock::leave(StackMachineThread* machine)
{
	this->unwindStack(machine,this->stackPos);
}

void CatchBlock::recycleInstance()
{
	this->returnInstance(this);
}

int CatchBlock::getAddress()
{
	return address;
}