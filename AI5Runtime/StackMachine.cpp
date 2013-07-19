#include "StackMachine.h"
#include <Windows.h>
#include <fstream>
#include "Instruction.h"
#include "encode.h"
#include <memory>
#include "..\AI5StandardLibrary\StandardLibrary.h"

StackMachine::StackMachine(shared_ptr<vector<shared_ptr<StaticData>>> statics,
					shared_ptr<vector<shared_ptr<Instruction>>> program,int startAddress): latestThread(0),staticsTable(statics),program(program),
					startAddress(startAddress), globalScope(GC::staticAlloc<Scope>()), macros(new std::unordered_map<UnicodeString,MACRO_FUNCTION,UnicodeStringHasher,UnicodeStringComparator>)
{
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
	for(auto it = threads.begin();it != threads.end();it++)
	{
		delete it->second;
	}
}


void StackMachine::startMainThread()
{
	mainThread = createThread(startAddress);
	mainThread->startThread();
}

StackMachineThread* StackMachine::createThread(int address)
{
	StackMachineThread* thread = new StackMachineThread(address,staticsTable,program,macros,globalScope);
	SM_THREAD_ID id = latestThread++;

	threads[id] = thread;

	return thread;
}

int StackMachine::waitForTermination()
{
	return mainThread->join();
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
