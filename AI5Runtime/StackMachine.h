#pragma once
#include "Instruction.h"
#include "Static.h"
#include <memory.h>
#include <list>
#include "Scope.h"
#include "StackMachineThread.h"
#include "gc.h"
#include "ThreadManager.h"

class StackMachine
{
public:
	friend class GC;
	StackMachine(shared_ptr<vector<shared_ptr<StaticData>>> statics,
					shared_ptr<vector<shared_ptr<Instruction>>> program,int startAddress);
	~StackMachine(void);
	
	VariantReference<Scope>& getGlobalScope();
	shared_ptr<vector<shared_ptr<Instruction>>> getCode();
	shared_ptr<vector<shared_ptr<StaticData>>> getStatics();
	shared_ptr<std::unordered_map<UnicodeString,MACRO_FUNCTION,UnicodeStringHasher,UnicodeStringComparator>> getMacros();
	
	void startMainThread();
	int waitForTermination();
	void disassembleProgram();
	void addBuiltInFunction(const UnicodeString &name,BuiltinFunction function);
	void addMacro(const UnicodeString &name,MACRO_FUNCTION macroFunc);
	ThreadManager* getThreadManager();
private:
	shared_ptr<vector<shared_ptr<Instruction>>> program;
	shared_ptr<vector<shared_ptr<StaticData>>> staticsTable;
	shared_ptr<std::unordered_map<UnicodeString,MACRO_FUNCTION,UnicodeStringHasher,UnicodeStringComparator>> macros;
	VariantReference<Scope> globalScope;
	
	ThreadContext* mainThread;
	ThreadManager threadManager;
	
	int startAddress;
};

