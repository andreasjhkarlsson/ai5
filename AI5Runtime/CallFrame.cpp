#include "CallFrame.h"
#include "StackMachine.h"

CallFrame::CallFrame(): Block(CALL_BLOCK)
{
}

CallFrame::~CallFrame(void)
{
}


void CallFrame::setup(StackMachine* machine,int returnAddress,int numberOfArguments,CallFrame* parentFrame)
{
	this->returnAddress = returnAddress;
	this->stackPosition = machine->getDataStack()->position()-(numberOfArguments+1);
	this->numberOfArguments = numberOfArguments;
	this->parentFrame = parentFrame;
	this->arguments.clear();
}

void CallFrame::leave(StackMachine*machine)
{
	unwindStack(machine,stackPosition);

	scope.reset();
}

Scope* CallFrame::getScope()
{
	return &scope;
}


void CallFrame::recycleInstance()
{
	CallFrame::returnInstance(this);
}

int CallFrame::getReturnAddress()
{
	return returnAddress;

}

CallFrame* CallFrame::getParentFrame()
{
	return this->parentFrame;
}


void CallFrame::addArgument(const Argument& arg)
{
	this->arguments.push_back(arg);
}

void CallFrame::loadArguments(StackMachine* machine,int total,int required)
{
	if(numberOfArguments > total)
		throw RuntimeError(L"Too many arguments in function call");
	if(numberOfArguments < required)
		throw RuntimeError(L"Too few arguments in function call");


	for(int argIndex=arguments.size()-1;argIndex>=0;argIndex--)
	{
		Argument arg = arguments[argIndex];
		Variant* varArg = machine->getDataStack()->pop();

		if(arg.isByref && varArg->getLastName() != nullptr)
		{
			machine->addNameToLocalScope(arg.identifier,varArg->getLastName());
		}
		else
		{
			machine->setLocal(arg.identifier,varArg);
		}

		varArg->release();
	}

}