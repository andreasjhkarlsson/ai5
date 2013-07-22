#pragma once
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>
#include <functional>
#include "Static.h"
#include "DataStack.h"
#include "Scope.h"
#include "CallBlock.h"
#include "macro.h"
#include "RuntimeError.h"
#include "SimplePool.h"
#include "BlockStack.h"
#include "BuiltinFunctionVariant.h"
#include "HandleVariant.h"
#include <thread>

class Instruction;

using std::shared_ptr;
using std::vector;

typedef unsigned int SM_THREAD_ID;



class ThreadHandle: public HandleVariant
{
public:
	static const HANDLE_TYPE HTYPE = THREAD_HANDLE;
	friend class GC;
	StackMachineThread* getMachineThread();
	bool isValid() const override;
private:
	ThreadHandle(StackMachine*,StackMachineThread*);
	~ThreadHandle();
	StackMachineThread* machineThread;
	StackMachine* machine;
};


// This represents the virtual machine.
// It controls program counter, stacks, tables, scopes and memory allocation.
class StackMachineThread
{
private:
	static const int BLOCK_STACK_SIZE		= 8192;
	static const int DATA_STACK_SIZE		= 32768;
public:

	friend class GC;

	StackMachineThread(StackMachine*,SM_THREAD_ID,shared_ptr<vector<shared_ptr<StaticData>>> statics,
					shared_ptr<vector<shared_ptr<Instruction>>> program,
					shared_ptr<std::unordered_map<UnicodeString,MACRO_FUNCTION,UnicodeStringHasher,UnicodeStringComparator>> macros,
					VariantReference<Scope>& globalScope);
	~StackMachineThread(void);

	// These methods are called by instructions, so they need
	// to be inlined for maximum speed.

	// Increment program counter by one.
	__forceinline void advanceCounter();
	// Increments or decrements program counter by specified amount.
	__forceinline void jumpRelative(int offset);
	// Set the program counter to specific address.
	__forceinline void jumpAbsolute(int position);
	// Returns program counter.
	__forceinline int getCurrentAddress();
	// Gets static data from index.
	__forceinline StaticData* getStaticData(int index);
	__forceinline DataStack* getDataStack();
	__forceinline BlockStack* getBlockStack();
	inline void setCurrentCallBlock(CallBlock* frame);
	inline CallBlock* getCurrentCallBlock();
	void startThread();
	void startThread(const VariantReference<UserFunctionVariant>& entryPoint);
	int join();
	void run();
	void terminate(int code);
	void forceKill();
	SM_THREAD_ID getThreadId();
	VariantReference<NameVariant> getNearestName(NameIdentifier identifier);
	VariantReference<NameVariant> getGlobalName(NameIdentifier identifier);
	VariantReference<NameVariant> getLocalName(NameIdentifier identifier);
	// Set the nearest name value (that means local name, then global). If no name is found
	// a name is created in the local scope (if available, otherwise global).
	void setNearest(NameIdentifier identifier,const VariantReference<>& variant,bool asConst=false);
	void setLocal(NameIdentifier identifier,const VariantReference<>& variant,bool asConst=false);
	void setGlobal(NameIdentifier identifier,const VariantReference<>& variant,bool asConst=false); 
	void addNameToLocalScope(NameIdentifier identifier,const VariantReference<NameVariant>& name);
	MACRO_FUNCTION getMacro(int staticIndex);

	const VariantReference<>& getErrorCode() const;
	const VariantReference<>& getExtendedCode() const;
	void setExtendedCode(const VariantReference<>&);
	void setErrorCode(const VariantReference<>&);

	int getReturnCode()
	{
		return returnCode;
	}

	void setThreadName(shared_string);
	shared_string getThreadName();

	GC::ThreadContext* threadContext;

private:
	// Code and static data.
	shared_ptr<vector<shared_ptr<Instruction>>> program;
	shared_ptr<vector<shared_ptr<StaticData>>> staticsTable;
	shared_ptr<std::unordered_map<UnicodeString,MACRO_FUNCTION,UnicodeStringHasher,UnicodeStringComparator>> macros;
	// Stores active blocks. Blocks can be loops, function calls, exception handler etc.
	BlockStack blockStack; 
	// Represents the current call frame.
	// Is not automatically set, so should be set by instructions
	// manipulating the block stack.
	CallBlock* currentCallBlock;
	// Macros are stored with a simple string as lookup.
	// TODO: Lookup macros with index as well.
	
	VariantReference<Scope>& globalScope;
	DataStack dataStack;
	// When set, the machine stops executing after next instruction.
	bool terminated;
	// Classic program counter.
	int programCounter;

	VariantReference<> errorCode;
	VariantReference<> extendedCode;

	int returnCode;

	std::thread* myThread;

	VariantReference<UserFunctionVariant> myThreadFunc;

	SM_THREAD_ID myId;

	StackMachine* owner;

	shared_string name;
};

void StackMachineThread::jumpRelative(int offset)
{
	programCounter += offset;
}
void StackMachineThread::jumpAbsolute(int position)
{
	programCounter = position;
}
StaticData* StackMachineThread::getStaticData(int index)
{
	return (*staticsTable)[index].get();
}

DataStack* StackMachineThread::getDataStack()
{
	return &dataStack;
}

BlockStack* StackMachineThread::getBlockStack()
{
	return &blockStack;
}

void StackMachineThread::advanceCounter()
{
	programCounter++;
}

int StackMachineThread::getCurrentAddress()
{
	return programCounter;
}

void StackMachineThread::setCurrentCallBlock(CallBlock* frame)
{
	this->currentCallBlock = frame;
}

CallBlock* StackMachineThread::getCurrentCallBlock()
{
	return this->currentCallBlock;
}

