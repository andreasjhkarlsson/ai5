#include <map>
#include "StackMachineThread.h"
#include "RuntimeError.h"
#include "NullVariant.h"
#include "Instruction.h"
#include "GlobalOptions.h"
#include <functional>
#include "misc.h"
#include "function_instructions.h"
#include "StackMachine.h"
#include <Windows.h>

using namespace std::placeholders;

StackMachineThread::StackMachineThread(StackMachine* owner,SM_THREAD_ID myId,shared_ptr<vector<shared_ptr<StaticData>>> statics,
	shared_ptr<vector<shared_ptr<Instruction>>> program,
	shared_ptr<std::unordered_map<UnicodeString,MACRO_FUNCTION,UnicodeStringHasher,UnicodeStringComparator>> macros,
	VariantReference<Scope>& globalScope): programCounter(0),
	dataStack(DATA_STACK_SIZE),staticsTable(statics),program(program), blockStack(BLOCK_STACK_SIZE), macros(macros),
	currentCallBlock(nullptr), globalScope(globalScope), errorCode(0), extendedCode(0), myId(myId), owner(owner)
{
	
}


ThreadHandle::ThreadHandle(StackMachine* machine,StackMachineThread* machineThread):
	HandleVariant(THREAD_HANDLE), machineThread(machineThread), machine(machine)
{

}
ThreadHandle::~ThreadHandle()
{
	delete machineThread;
}
StackMachineThread* ThreadHandle::getMachineThread()
{
	return machineThread;
}

bool ThreadHandle::isValid() const
{
	return machineThread != nullptr;
}

StackMachineThread::~StackMachineThread(void)
{
	
}

void StackMachineThread::startThread()
{
	
	myThread = new std::thread(std::bind(&StackMachineThread::run,this));
}



void StackMachineThread::startThread(const VariantReference<UserFunctionVariant>& func)
{
	myThreadFunc = func;

	// Call func by manually invoking instruction.
	// Note that the callFunction instruction will
	// only setup environment for function call, no real work is done.
	dataStack.push(func);
	callFunction(this,0);

	// CurrentCallBlock should be set by above instruction.
	currentCallBlock->terminateOnReturn();

	myThread = new std::thread(std::bind(&StackMachineThread::run,this));
}


int StackMachineThread::join()
{
	myThread->join();
	delete myThread;
	return returnCode;
}

SM_THREAD_ID StackMachineThread::getThreadId()
{
	return myId;
}

void StackMachineThread::run()
{
	GC::initThread(this);
	terminated = false;
	try
	{
		while (!terminated) 
		{
			if(GlobalOptions::shouldPrintInstructions())
			{
				(*program)[programCounter]->print(staticsTable);
			}
			
			(*program)[programCounter]->execute(this);		
		}
	}
	catch(RuntimeError& error)
	{
		std::wcout << L"Runtime error:" << std::endl << error.getMessage().getTerminatedBuffer() <<
			std::endl << "The program will now terminate." << std::endl;
		returnCode = -1;
	}
	GC::uninitThread();
	owner->reportThreadTermination(myId);

}

void StackMachineThread::terminate(int code)
{
	terminated = true;
	returnCode = code;
}

void StackMachineThread::forceKill()
{
	TerminateThread(myThread->native_handle(),-1);
}



MACRO_FUNCTION StackMachineThread::getMacro(int staticIndex)
{
	return (*macros)[*std::static_pointer_cast<StaticMacro>((*staticsTable)[staticIndex])->getName()];
}


VariantReference<NameVariant> StackMachineThread::getNearestName(NameIdentifier identifier)
{
	if(currentCallBlock != nullptr)
	{
		VariantReference<NameVariant> name = currentCallBlock->getScope()->getNameFromIndex(identifier.localId);
		if(!name.empty())
			return name;
	}

	VariantReference<NameVariant> name = globalScope->getNameFromIndex(identifier.globalId);

	// If name not found from index, do a "hard" search with the name
	// Add it as an index afterwards so next lookup is FAST.
	if(name.empty())
	{
		std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier.staticId];
		name = globalScope->getNameFromString(*std::static_pointer_cast<StaticName>(staticData)->getName());
		// If name is still nullptr, throw error!
		globalScope->createIndexForName(*std::static_pointer_cast<StaticName>(staticData)->getName(),identifier.globalId);
	}

	return name;
}

VariantReference<NameVariant> StackMachineThread::getLocalName(NameIdentifier identifier)
{
	VariantReference<Scope> scope = globalScope;
	if(currentCallBlock != nullptr)
		scope = currentCallBlock->getScope();
	return scope->getNameFromIndex(identifier.localId);
}

VariantReference<NameVariant> StackMachineThread::getGlobalName(NameIdentifier identifier)
{
	return globalScope->getNameFromIndex(identifier.globalId);
}

// This function sets the value for a name in the nearest scope where it's found.
// If it isn't found it is added to the local scope, and if there is no local scope, to the global scope.
void StackMachineThread::setNearest(NameIdentifier identifier,const VariantReference<>& variant,bool asConst)
{
	// Search for name in local and global scope.
	VariantReference<NameVariant> foundName;
	if(currentCallBlock != nullptr)
		foundName = currentCallBlock->getScope()->getNameFromIndex(identifier.localId);
	if(foundName.empty())
		foundName = globalScope->getNameFromIndex(identifier.globalId);

	// If not found, add it as a new name to the nearest scope.
	if(foundName.empty())
	{
		VariantReference<Scope> targetScope = globalScope;
		if(currentCallBlock != nullptr)
			targetScope = currentCallBlock->getScope();

		std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier.staticId];

		// The name may be defined without this index. This doesn't matter as the createIndexForName will check
		// if the name is already defined.
		foundName = targetScope->createIndexForName(*std::static_pointer_cast<StaticName>(staticData)->getName(),identifier.localId);
	}

	
	foundName->setValue(variant);

	if(asConst)
	{
		foundName->markAsConst();
	}
}


void StackMachineThread::setLocal(NameIdentifier identifier,const VariantReference<>& variant,bool asConst)
{
	VariantReference<Scope> targetScope = globalScope;
	if(currentCallBlock != nullptr)
		targetScope = currentCallBlock->getScope();

	VariantReference<NameVariant> name = targetScope->getNameFromIndex(identifier.localId);

	if(name.empty())
	{
		std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier.staticId];
		name = targetScope->createIndexForName(*std::static_pointer_cast<StaticName>(staticData)->getName(),identifier.localId);
	}

	name->setValue(variant);

	if(asConst)
	{
		name->markAsConst();
	}

}
void StackMachineThread::setGlobal(NameIdentifier identifier,const VariantReference<>& variant,bool asConst)
{
	VariantReference<NameVariant> foundName = globalScope->getNameFromIndex(identifier.globalId);
	if(foundName.empty())
	{		
		std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier.staticId];
		foundName = globalScope->createIndexForName(*std::static_pointer_cast<StaticName>(staticData)->getName(),identifier.globalId);
	}
	foundName->setValue(variant);

	if(asConst)
	{
		foundName->markAsConst();
	}
}

void StackMachineThread::addNameToLocalScope(NameIdentifier identifier,const VariantReference<NameVariant>& name)
{
	VariantReference<Scope> targetScope = globalScope;
	if(currentCallBlock != nullptr)
		targetScope = currentCallBlock->getScope();

	std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier.staticId];
	const UnicodeString& strName = *std::static_pointer_cast<StaticName>(staticData)->getName();
	targetScope->insertName(strName,identifier.localId,name);
}

const VariantReference<>& StackMachineThread::getErrorCode() const
{
	return errorCode;
}
const VariantReference<>& StackMachineThread::getExtendedCode() const
{
	return extendedCode;
}
void StackMachineThread::setExtendedCode(const VariantReference<>& extendedCode)
{
	this->extendedCode = extendedCode;
}
void StackMachineThread::setErrorCode(const VariantReference<>& errorCode)
{
	this->errorCode = errorCode;
}