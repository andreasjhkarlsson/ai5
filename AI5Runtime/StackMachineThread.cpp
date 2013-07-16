#include <map>
#include "StackMachineThread.h"
#include "RuntimeError.h"
#include "NullVariant.h"
#include "Instruction.h"
#include "GlobalOptions.h"
#include <functional>

using namespace std::placeholders;

StackMachineThread::StackMachineThread(int address,shared_ptr<vector<shared_ptr<StaticData>>> statics,
	shared_ptr<vector<shared_ptr<Instruction>>> program,
	shared_ptr<std::unordered_map<UnicodeString,MACRO_FUNCTION,UnicodeStringHasher,UnicodeStringComparator>> macros,
	Scope* globalScope): programCounter(0),
	dataStack(DATA_STACK_SIZE),staticsTable(statics),program(program), blockStack(BLOCK_STACK_SIZE), macros(macros),startAddress(address),
	currentCallBlock(nullptr), globalScope(globalScope), errorCode(new Integer32Variant(0)), extendedCode(new Integer32Variant(0))
{
	globalScope->addRef();
}

StackMachineThread::~StackMachineThread(void)
{
	globalScope->release();
	errorCode->release();
	extendedCode->release();
}

void StackMachineThread::startThread()
{
	
	myThread = new std::thread(std::bind(&StackMachineThread::run,this));
}


int StackMachineThread::join()
{
	myThread->join();
	return returnCode;
}

void StackMachineThread::run()
{
	terminated = false;
	try
	{
		while (!terminated) 
		{
			if(GlobalOptions::isVerbose())
			{
				std::wcout << "\t";
				(*program)[programCounter]->format(std::wcout,staticsTable);
				std::wcout << std::endl;
			}
			
			(*program)[programCounter]->execute(this);		
		}
		returnCode = dataStack.top()->toInteger32();
	}
	catch(RuntimeError& error)
	{
		std::wcout << L"Runtime error:" << std::endl << error.getMessage().getTerminatedBuffer() <<
			std::endl << "The program will now terminate." << std::endl;
		returnCode = -1;
	}

}

void StackMachineThread::terminate()
{
	terminated = true;
}



MACRO_FUNCTION StackMachineThread::getMacro(int staticIndex)
{
	return (*macros)[*std::static_pointer_cast<StaticMacro>((*staticsTable)[staticIndex])->getName()];
}


NameVariant* StackMachineThread::getNearestName(NameIdentifier identifier)
{
	if(currentCallBlock != nullptr)
	{
		NameVariant* name = currentCallBlock->getScope()->getNameFromIndex(identifier.localId);
		if(name != nullptr)
			return name;
	}

	NameVariant* name = globalScope->getNameFromIndex(identifier.globalId);

	// If name not found from index, do a "hard" search with the name
	// Add it as an index afterwards so next lookup is FAST.
	if(name == nullptr)
	{
		std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier.staticId];
		name = globalScope->getNameFromString(*std::static_pointer_cast<StaticName>(staticData)->getName());
		// If name is still nullptr, throw error!
		globalScope->createIndexForName(&variantFactory,*std::static_pointer_cast<StaticName>(staticData)->getName(),identifier.globalId);
	}

	return name;
}

NameVariant* StackMachineThread::getLocalName(NameIdentifier identifier)
{
	Scope* scope = globalScope;
	if(currentCallBlock != nullptr)
		scope = currentCallBlock->getScope();
	return scope->getNameFromIndex(identifier.localId);
}

NameVariant* StackMachineThread::getGlobalName(NameIdentifier identifier)
{
	Scope* scope = globalScope;
	return scope->getNameFromIndex(identifier.globalId);
}

// This function sets the value for a name in the nearest scope where it's found.
// If it isn't found it is added to the local scope, and if there is no local scope, to the global scope.
void StackMachineThread::setNearest(NameIdentifier identifier,Variant* variant,bool asConst)
{
	// Search for name in local and global scope.
	NameVariant* foundName = nullptr;
	if(currentCallBlock != nullptr)
		foundName = currentCallBlock->getScope()->getNameFromIndex(identifier.localId);
	if(foundName == nullptr)
		foundName = globalScope->getNameFromIndex(identifier.globalId);

	// If not found, add it as a new name to the nearest scope.
	if(foundName == nullptr)
	{
		Scope* targetScope = nullptr;
		if(currentCallBlock != nullptr)
			targetScope = currentCallBlock->getScope();
		else
			targetScope = globalScope;
		std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier.staticId];

		// The name may be defined without this index. This doesn't matter as the createIndexForName will check
		// if the name is already defined.
		foundName = targetScope->createIndexForName(&variantFactory,*std::static_pointer_cast<StaticName>(staticData)->getName(),identifier.localId);
	}


	foundName->setValue(variant);

	if(asConst)
	{
		foundName->markAsConst();
	}
}


void StackMachineThread::setLocal(NameIdentifier identifier,Variant* variant,bool asConst)
{
	Scope* targetScope = globalScope;
	if(currentCallBlock != nullptr)
		targetScope = currentCallBlock->getScope();

	NameVariant* name = targetScope->getNameFromIndex(identifier.localId);

	if(name == nullptr)
	{
		std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier.staticId];
		name = targetScope->createIndexForName(&variantFactory,*std::static_pointer_cast<StaticName>(staticData)->getName(),identifier.localId);
	}

	name->setValue(variant);

	if(asConst)
	{
		name->markAsConst();
	}

}
void StackMachineThread::setGlobal(NameIdentifier identifier,Variant* variant,bool asConst)
{
	NameVariant* foundName = globalScope->getNameFromIndex(identifier.globalId);
	if(foundName == nullptr)
	{		
		std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier.staticId];
		foundName = globalScope->createIndexForName(&variantFactory,*std::static_pointer_cast<StaticName>(staticData)->getName(),identifier.globalId);
	}
	foundName->setValue(variant);

	if(asConst)
	{
		foundName->markAsConst();
	}
}

void StackMachineThread::addNameToLocalScope(NameIdentifier identifier,NameVariant* name)
{
	Scope* targetScope = globalScope;
	if(currentCallBlock != nullptr)
		targetScope = currentCallBlock->getScope();

	std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier.staticId];
	const UnicodeString& strName = *std::static_pointer_cast<StaticName>(staticData)->getName();
	targetScope->insertName(strName,identifier.localId,name);
}



Variant* StackMachineThread::getErrorCode()
{
	return errorCode;
}
Variant* StackMachineThread::getExtendedCode()
{
	return extendedCode;
}
void StackMachineThread::setExtendedCode(Variant* extendedCode)
{
	if(this->extendedCode != nullptr)
		this->extendedCode->release();
	this->extendedCode = extendedCode;
	if(this->extendedCode != nullptr)
		this->extendedCode->addRef();
}
void StackMachineThread::setErrorCode(Variant* extendedCode)
{
	if(this->errorCode != nullptr)
		this->errorCode->release();
	this->errorCode = extendedCode;
	if(this->errorCode != nullptr)
		this->errorCode->addRef();
}