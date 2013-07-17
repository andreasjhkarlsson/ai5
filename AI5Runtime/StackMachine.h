#pragma once
#include "Instruction.h"
#include "Static.h"
#include <memory.h>
#include <list>
#include "Scope.h"
#include "StackMachineThread.h"
class StackMachine
{
public:
	StackMachine(shared_ptr<vector<shared_ptr<StaticData>>> statics,
					shared_ptr<vector<shared_ptr<Instruction>>> program,int startAddress);
	~StackMachine(void);
	StackMachineThread* createThread(int address);
	void startMainThread();
	int waitForTermination();
	void disassembleProgram();
	void addBuiltInFunction(const UnicodeString &name,BuiltinFunction function);
	void addMacro(const UnicodeString &name,MACRO_FUNCTION macroFunc);
private:
	shared_ptr<vector<shared_ptr<Instruction>>> program;
	shared_ptr<vector<shared_ptr<StaticData>>> staticsTable;
	shared_ptr<std::unordered_map<UnicodeString,MACRO_FUNCTION,UnicodeStringHasher,UnicodeStringComparator>> macros;
	VariantReference<Scope> globalScope;
	std::unordered_map<SM_THREAD_ID,StackMachineThread*> threads;
	StackMachineThread* mainThread;
	SM_THREAD_ID latestThread;
	int startAddress;
};

