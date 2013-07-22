#include "CallBlock.h"
#include "StackMachineThread.h"
#include "DefaultVariant.h"
#include "UserFunctionVariant.h"
#include "NameReferenceVariant.h"

CallBlock::CallBlock(): Block(CALL_BLOCK)
{
}

CallBlock::~CallBlock(void)
{
}

void CallBlock::setup(StackMachineThread* machine,int returnAddress,int calledNumberOfArguments,CallBlock* parentFrame,VariantReference<UserFunctionVariant>& owner)
{
	this->scope = Scope::Create();
	this->returnAddress = returnAddress;
	this->stackPosition = machine->getDataStack()->size()-(calledNumberOfArguments+1);
	this->calledNumberOfArguments = calledNumberOfArguments;
	this->parentFrame = parentFrame;
	this->arguments.clear();
	this->closures.clear();
	this->closureScope = VariantReference<Scope>();
	this->closedNames.clear();
	this->owner = owner.cast<UserFunctionVariant>();
	this->shouldTerminateOnReturn = false;
}


void CallBlock::terminateOnReturn()
{
	this->shouldTerminateOnReturn = true;
}

void CallBlock::leave(StackMachineThread*machine)
{
	unwindStack(machine,stackPosition);

	processClosures(machine);
	scope.clear();
	closureScope.clear();
	closures.clear();
	owner.clear();
	if(shouldTerminateOnReturn)
	{
		// TODO: Find real value here somehow.
		machine->terminate(0);
	}
}

// When returning from a function, there needs to be certain stuff
// done to the closures created inside the call.
// The most important one being unlinking the closures to the current scope
// and creating new scopes with selected names attached only.
// This avoids circular references if the closures doesn't reference themselves
// and makes sure variable not used by closures are freed immeditely.
void CallBlock::processClosures(StackMachineThread* machine)
{

	if(closureScope.empty())
		return;

	// For each name that was added as a "closed name" during call
	// add it to the closure scope.
	for(size_t i=0;i<closedNames.size();i++)
	{
		NameIdentifier id = closedNames[i];
		VariantReference<NameVariant>& name = scope->getNameFromIndex(id.localId);
		if(!name.empty())
		{
			StaticName* staticName = static_cast<StaticName*>(machine->getStaticData(id.staticId));
			closureScope->insertName(*staticName->getName(),id.localId,name);
		}
	}


	// Relink closures scope into the proper scope.
	for(size_t i=0;i<closures.size();i++)
	{
		closures[i]->getEnclosingScope()->setEnclosingScope(owner->getEnclosingScope());
	}

}


VariantReference<Scope>& CallBlock::getScope()
{
	return scope;
}

void CallBlock::recycleInstance()
{
	CallBlock::returnInstance(this);
}

int CallBlock::getReturnAddress()
{
	return returnAddress;
}

CallBlock* CallBlock::getParentFrame()
{
	return this->parentFrame;
}

void CallBlock::addArgument(const Argument& arg)
{
	this->arguments.push_back(arg);
}

void CallBlock::loadArguments(StackMachineThread* machine,int total,int required)
{
	if(calledNumberOfArguments > total || calledNumberOfArguments < required)
		throw InvalidArgumentCountError(calledNumberOfArguments,required,total);

	for(size_t argIndex=arguments.size();argIndex --> 0 ;)
	{
		Argument arg = arguments[argIndex];

		VariantReference<> varArg;

		// Set arguments with missing value with Default.
		// The real default value is then set by the function body.
		if(argIndex >= (size_t)calledNumberOfArguments)
		{
			varArg = VariantReference<>::DefaultReference();
		}
		else
		{
			varArg = machine->getDataStack()->pop();
		}

		if(varArg.isNameType() || varArg.isNameReferenceType())
		{
			if(arg.isByref)
			{
				machine->addNameToLocalScope(arg.identifier,GC::alloc<NameReferenceVariant,const VariantReference<>&>(nullptr));
			}
			else
			{
				machine->setLocal(arg.identifier,varArg.cast<NameVariant>()->getValue());
			}

		}
		else
		{
			machine->setLocal(arg.identifier,varArg);
		}

	}

	VariantReference<UserFunctionVariant> self = machine->getDataStack()->pop().cast<UserFunctionVariant>();

	this->getScope()->setEnclosingScope(self->getEnclosingScope());

}


void CallBlock::addClosedName(StackMachineThread* machine,NameIdentifier nameIdentifier)
{
	closedNames.push_back(nameIdentifier);
}

void CallBlock::addClosure(StackMachineThread* machine,VariantReference<UserFunctionVariant>& closure)
{

	if(closureScope.empty())
		closureScope = Scope::Create();

	closureScope->setEnclosingScope(scope);

	closure->setEnclosingScope(closureScope);
	closures.push_back(closure);
}