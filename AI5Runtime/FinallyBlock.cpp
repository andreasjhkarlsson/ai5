#include "FinallyBlock.h"
#include "StackMachineThread.h"

FinallyBlock::FinallyBlock(): Block(FINALLY_BLOCK)
{
}


FinallyBlock::~FinallyBlock(void)
{
}

void FinallyBlock::setup(StackMachineThread* machine,int address)
{
	this->address = address;
	this->stackPos = machine->getDataStack()->size();
	executing = false;
}
void FinallyBlock::leave(StackMachineThread* machine)
{
	this->unwindStack(machine,stackPos);
}

void FinallyBlock::recycleInstance()
{
	this->returnInstance(this);
}
void FinallyBlock::setRethrowAction(const VariantReference<> ex)
{
	returnInfo.action = ReturnInfo::THROW_EXCEPTION;
	returnInfo.exceptionToThrow = ex;
}
void FinallyBlock::setFunctionReturnAction(const VariantReference<>& returnValue)
{
	returnInfo.action = ReturnInfo::RETURN_FROM_FUNC;
	returnInfo.returnValue = returnValue;
}
void FinallyBlock::setJumpToAction(int address)
{
	returnInfo.action = ReturnInfo::JUMP_TO;
	returnInfo.returnAddress = address;
}

void FinallyBlock::setContinueLoopAction(int level)
{
	returnInfo.action = ReturnInfo::CONTINUELOOP;
	returnInfo.level = level;
}

void FinallyBlock::setExitLoopAction(int level)
{
	returnInfo.action = ReturnInfo::EXITLOOP;
	returnInfo.level = level;
}

void  FinallyBlock::execute(StackMachineThread* machine)
{

	if(executing)
	{
		throw RuntimeError(L"Control flow may not leave finally block!");
	}

	machine->getBlockStack()->push(this);
	machine->jumpAbsolute(address);
	executing = true;

}

const FinallyBlock::ReturnInfo& FinallyBlock::getReturnInfo()
{
	return returnInfo;
}