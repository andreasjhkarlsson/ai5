#include "StackMachine.h"
#include <Windows.h>
#include <fstream>
#include "Instruction.h"
#include "encode.h"
#include <memory>
#include "..\AI5StandardLibrary\StandardLibrary.h"
#include "misc.h"

StackMachine::StackMachine(shared_ptr<vector<shared_ptr<StaticData>>> statics,
					shared_ptr<vector<shared_ptr<Instruction>>> program,int startAddress): staticsTable(statics),program(program),
					startAddress(startAddress), macros(new std::unordered_map<UnicodeString,MACRO_FUNCTION,UnicodeStringHasher,UnicodeStringComparator>)
{
	DebugOut(L"Virtual machine") << "Initializing GC";
	GC::init(this);
	globalScope = GC::staticAlloc<Scope,const wchar_t*>(L"global");
	DebugOut(L"Virtual machine") << "Loading standard library";
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
	StackMachineThread* virtualThread = new StackMachineThread(this);
	virtualThread->jumpAbsolute(startAddress);
	mainThread = threadManager.createThread(this,create_shared_string(L"main"));
	mainThread->start();
}

int StackMachine::waitForTermination()
{
	int retCode = mainThread->join();

	return retCode;
}

void StackMachine::disassembleProgram()
{
	for(size_t i=0;i<program->size();i++)
	{
		std::wcout << i << ": ";
		(*program)[i]->print(staticsTable);
	}
}

ThreadManager* StackMachine::getThreadManager()
{
	return &threadManager;
}

VariantReference<Scope>& StackMachine::getGlobalScope()
{
	return globalScope;
}
shared_ptr<vector<shared_ptr<Instruction>>> StackMachine::getCode()
{
	return program;
}
shared_ptr<vector<shared_ptr<StaticData>>> StackMachine::getStatics()
{
	return staticsTable;
}
shared_ptr<std::unordered_map<UnicodeString,MACRO_FUNCTION,UnicodeStringHasher,UnicodeStringComparator>> StackMachine::getMacros()
{
	return macros;
}