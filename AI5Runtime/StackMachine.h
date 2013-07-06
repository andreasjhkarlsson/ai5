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

class Instruction;

using std::shared_ptr;
using std::vector;

// This represents the virtual machine.
// It controls program counter, stacks, tables, scopes and memory allocation.
class StackMachine
{
private:
	static const int BLOCK_STACK_SIZE		= 8192;
	static const int DATA_STACK_SIZE		= 32768;
public:
	StackMachine(shared_ptr<vector<shared_ptr<StaticData>>> statics,
					shared_ptr<vector<shared_ptr<Instruction>>> program);
	~StackMachine(void);

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
	__forceinline VariantFactory* getVariantFactory();
	__forceinline BlockStack* getBlockStack();
	inline void setCurrentCallBlock(CallBlock* frame);
	inline CallBlock* getCurrentCallBlock();
	int start();
	void terminate();
	void disassemble();
	void setVerbose();
	NameVariant* getNearestName(NameIdentifier identifier);
	NameVariant* getGlobalName(NameIdentifier identifier);
	NameVariant* getLocalName(NameIdentifier identifier);
	// Set the nearest name value (that means local name, then global). If no name is found
	// a name is created in the local scope (if available, otherwise global).
	void setNearest(NameIdentifier identifier,Variant* variant,bool asConst=false);
	void setLocal(NameIdentifier identifier,Variant* variant,bool asConst=false);
	void setGlobal(NameIdentifier identifier,Variant* variant,bool asConst=false); 
	void addNameToLocalScope(NameIdentifier identifier,NameVariant* name);
	void addBuiltInFunction(const UnicodeString &name,BuiltinFunction function);
	void addMacro(const UnicodeString &name,MACRO_FUNCTION macroFunc);
	MACRO_FUNCTION getMacro(int staticIndex);

	Variant* getErrorCode();
	Variant* getExtendedCode();
	void setExtendedCode(Variant*);
	void setErrorCode(Variant*);

private:
	// Code and static data.
	shared_ptr<vector<shared_ptr<Instruction>>> program;
	shared_ptr<vector<shared_ptr<StaticData>>> staticsTable;
	// Stores active blocks. Blocks can be loops, function calls, exception handler etc.
	BlockStack blockStack; 
	// Represents the current call frame.
	// Is not automatically set, so should be set by instructions
	// manipulating the block stack.
	CallBlock* currentCallBlock;
	// Macros are stored with a simple string as lookup.
	// TODO: Lookup macros with index as well.
	std::unordered_map<UnicodeString,MACRO_FUNCTION,UnicodeStringHasher,UnicodeStringComparator> macros;
	Scope *globalScope;
	DataStack dataStack;
	VariantFactory variantFactory;
	// When set, the machine stops executing after next instruction.
	bool terminated;
	// Classic program counter.
	int programCounter;
	bool verbose;


	Variant* errorCode;
	Variant* extendedCode;
};

void StackMachine::jumpRelative(int offset)
{
	programCounter += offset;
}
void StackMachine::jumpAbsolute(int position)
{
	programCounter = position;
}
StaticData* StackMachine::getStaticData(int index)
{
	return (*staticsTable)[index].get();
}

DataStack* StackMachine::getDataStack()
{
	return &dataStack;
}

BlockStack* StackMachine::getBlockStack()
{
	return &blockStack;
}

void StackMachine::advanceCounter()
{
	programCounter++;
}

VariantFactory* StackMachine::getVariantFactory()
{
	return &variantFactory;
}

int StackMachine::getCurrentAddress()
{
	return programCounter;
}

void StackMachine::setCurrentCallBlock(CallBlock* frame)
{
	this->currentCallBlock = frame;
}

CallBlock* StackMachine::getCurrentCallBlock()
{
	return this->currentCallBlock;
}

