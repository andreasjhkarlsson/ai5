#include "CallBlock.h"
#include "StackMachine.h"
#include "DefaultVariant.h"
#include "UserFunctionVariant.h"
#include "NameReferenceVariant.h"

CallBlock::CallBlock(): Block(CALL_BLOCK)
{
}

CallBlock::~CallBlock(void)
{
}

void CallBlock::setup(StackMachine* machine,int returnAddress,int calledNumberOfArguments,CallBlock* parentFrame,UserFunctionVariant* owner)
{
	this->scope = Scope::createFromFactory(machine->getVariantFactory());
	this->returnAddress = returnAddress;
	this->stackPosition = machine->getDataStack()->position()-(calledNumberOfArguments+1);
	this->calledNumberOfArguments = calledNumberOfArguments;
	this->parentFrame = parentFrame;
	this->arguments.clear();
	this->closures.clear();
	this->closureScope = nullptr;
	this->closedNames.clear();
	this->owner = owner;
	this->owner->addRef();
}

void CallBlock::leave(StackMachine*machine)
{
	unwindStack(machine,stackPosition);

	processClosures(machine);

	scope->release();

	this->owner->release();
}

// When returning from a function, there needs to be certain stuff
// done to the closures created inside the call.
// The most important one being unlinking the closures to the current scope
// and creating new scopes with selected names attached only.
// This avoids circular references if the closures doesn't reference themselves
// and makes sure variable not used by closures are freed immeditely.
void CallBlock::processClosures(StackMachine* machine)
{
	// For each name that was added as a "closed name" during call
	// add it to the closure scope.
	for(int i=0;i<closedNames.size();i++)
	{
		NameIdentifier id = closedNames[i];
		NameVariant* name = scope->getNameFromIndex(id.localId);
		if(name != nullptr)
		{
			StaticName* staticName = static_cast<StaticName*>(machine->getStaticData(id.staticId));
			closureScope->insertName(*staticName->getName(),id.localId,name);
		}
	}

	if(closureScope != nullptr)
	{
		// Relink closures scope into the proper scope.
		for(int i=0;i<closures.size();i++)
		{
			closures[i]->getEnclosingScope()->setEnclosingScope(owner->getEnclosingScope());
			closures[i]->release();
		}
		// Release the closure scope.
		// If any closure have escaped the function the scope will live on.
		closureScope->release();
	}
}


Scope* CallBlock::getScope()
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

void CallBlock::loadArguments(StackMachine* machine,int total,int required)
{
	if(calledNumberOfArguments > total)
		throw RuntimeError(L"Too many arguments in function call");
	if(calledNumberOfArguments < required)
		throw RuntimeError(L"Too few arguments in function call");

	for(size_t argIndex=arguments.size();argIndex --> 0 ;)
	{
		Argument arg = arguments[argIndex];

		Variant* varArg;

		// Set arguments with missing value with Default.
		// The real default value is then set by the function body.
		if(argIndex >= calledNumberOfArguments)
		{
			varArg = &DefaultVariant::Instance;
			varArg->addRef();
		}
		else
		{
			varArg = machine->getDataStack()->pop();
		}

		if(varArg->isNameType() || varArg->isNameReferenceType())
		{
			if(arg.isByref)
			{
				NameReferenceVariant* ref = machine->getVariantFactory()->create<NameReferenceVariant,NameVariant*>(Variant::NAME_REFERENCE,static_cast<NameVariant*>(varArg));
				machine->addNameToLocalScope(arg.identifier,ref);
			}
			else
			{
				machine->setLocal(arg.identifier,static_cast<NameVariant*>(varArg)->getValue());
			}

		}
		else
		{
			machine->setLocal(arg.identifier,varArg);
		}

		varArg->release();
	}

	UserFunctionVariant* self = static_cast<UserFunctionVariant*>(machine->getDataStack()->pop());

	this->getScope()->setEnclosingScope(self->getEnclosingScope());

	self->release();

}


void CallBlock::addClosedName(StackMachine* machine,NameIdentifier nameIdentifier)
{
	closedNames.push_back(nameIdentifier);
}

void CallBlock::addClosure(StackMachine* machine,UserFunctionVariant* closure)
{
	closure->addRef();

	if(closureScope == nullptr)
		closureScope = Scope::createFromFactory(machine->getVariantFactory());

	closureScope->setEnclosingScope(scope);

	closure->setEnclosingScope(closureScope);
	closures.push_back(closure);
}