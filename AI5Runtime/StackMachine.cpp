#include "StackMachine.h"
#include <Windows.h>
#include <fstream>
#include "Instruction.h"
#include "encode.h"
#include <memory>
#include "..\AI5StandardLibrary\StandardLibrary.h"
#include "misc.h"

StackMachine::StackMachine(shared_ptr<vector<shared_ptr<StaticData>>> statics,
					shared_ptr<vector<shared_ptr<Instruction>>> program,int startAddress): latestThread(0),staticsTable(statics),program(program),
					startAddress(startAddress), macros(new std::unordered_map<UnicodeString,MACRO_FUNCTION,UnicodeStringHasher,UnicodeStringComparator>)
{
	
	GC::init(this);
	GC::initThread(nullptr);
	globalScope = GC::staticAlloc<Scope>();
	registerStandardLibrary(this);
	
}

void StackMachine::addBuiltInFunction(const UnicodeString &name,BuiltinFunction function)
{
	globalScope.cast<Scope>()->createName(name).cast<NameVariant>()->setValue(BuiltinFunctionVariant::CreateStatic(name,function));
}

void StackMachine::addMacro(const UnicodeString &name,MACRO_FUNCTION macroFunc)
{
	(*macros)[name] = macroFunc;
}

StackMachine::~StackMachine(void)
{

}

void StackMachine::startMainThread()
{
	mainThread = createThread();
	mainThread->getMachineThread()->jumpAbsolute(startAddress);
	mainThread->getMachineThread()->startThread();
}

VariantReference<ThreadHandle> StackMachine::createThread()
{
	std::lock_guard<std::mutex> guard(threadsLock);

	SM_THREAD_ID id = latestThread++;
	StackMachineThread* thread = new StackMachineThread(this,id,staticsTable,program,macros,globalScope);

	VariantReference<ThreadHandle> threadRef = GC::alloc<ThreadHandle,StackMachine*,StackMachineThread*>(this,thread);

	threads[id] = threadRef;

	return threadRef;
}

int StackMachine::waitForTermination()
{
	int retCode = mainThread->getMachineThread()->join();

	return retCode;
}


void StackMachine::terminateAllThreads()
{
	threadsLock.lock();
	for(auto it = threads.begin();it != threads.end();it++)
	{

		it->second->getMachineThread()->terminate(-1);
	}
	threadsLock.unlock();
	// Give the threads some time to terminate by themselves.
	Sleep(50);

	// Threads still in ::threads have still not terminated. Kill them with fire!
	threadsLock.lock();
	for(auto it= threads.begin();it != threads.end(); it++)
	{
		DebugOut(L"Virtual machine") << L"Forcefully killing thread with id: " << it->first;
		it->second->getMachineThread()->forceKill();
	}
	threads.clear();
	threadsLock.unlock();
}

void StackMachine::reportThreadTermination(SM_THREAD_ID threadId)
{
	std::lock_guard<std::mutex> guard(threadsLock);
	threads.erase(threadId);
}


void StackMachine::disassembleProgram()
{
	for(size_t i=0;i<program->size();i++)
	{
		std::wcout << i << ": ";
		(*program)[i]->print(staticsTable);
		std::wcout << std::endl;
	}
}
