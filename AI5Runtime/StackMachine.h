#pragma once
#include "Instruction.h"
#include "Static.h"
#include <memory.h>
#include <list>
#include "Scope.h"
#include "StackMachineThread.h"
#include "gc.h"
#include <mutex>
class StackMachine
{
public:
	friend class GC;
	StackMachine(shared_ptr<vector<shared_ptr<StaticData>>> statics,
					shared_ptr<vector<shared_ptr<Instruction>>> program,int startAddress);
	~StackMachine(void);
	VariantReference<ThreadHandle> createThread();
	void reportThreadTermination(SM_THREAD_ID);
	void startMainThread();
	int waitForTermination();
	void terminateAllThreads();
	void disassembleProgram();
	void addBuiltInFunction(const UnicodeString &name,BuiltinFunction function);
	void addMacro(const UnicodeString &name,MACRO_FUNCTION macroFunc);
private:
	shared_ptr<vector<shared_ptr<Instruction>>> program;
	shared_ptr<vector<shared_ptr<StaticData>>> staticsTable;
	shared_ptr<std::unordered_map<UnicodeString,MACRO_FUNCTION,UnicodeStringHasher,UnicodeStringComparator>> macros;
	VariantReference<Scope> globalScope;
	std::unordered_map<SM_THREAD_ID,VariantReference<ThreadHandle>> threads;
	VariantReference<ThreadHandle> mainThread;
	SM_THREAD_ID latestThread;
	int startAddress;
	std::mutex threadsLock;
};

