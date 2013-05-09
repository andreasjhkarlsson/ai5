#pragma once

#include <vector>
#include <memory>
#include <string>
#include <map>
#include "Static.h"
#include "DataStack.h"
#include "NameStorage.h"
#include "CallFrame.h"
#include "BuiltinFunctionVariant.h"

class Instruction;

class StackMachine
{
public:
	StackMachine(std::shared_ptr<std::vector<STATIC_DATA*>> statics,
					std::shared_ptr<std::vector<Instruction*>> program);
	~StackMachine(void);
	// These methods are called by instructions, so they need
	// to be inlined for maximum speed.
	__forceinline void advanceCounter();
	__forceinline void jumpRelative(int offset);
	__forceinline void jumpAbsolute(int position);
	__forceinline int getCurrentAddress();
	__forceinline STATIC_DATA* getStaticData(int index);
	__forceinline DataStack* getDataStack();
	__forceinline VariantFactory* getVariantFactory();
	static StackMachine* LoadFromStructuredData(const std::string& filename);
	void start();
	void terminate();
	__forceinline void pushCallFrame(int returnAddress);
	__forceinline NameStorage* getNameStorage()
	{
		return &nameStorage;
	}
	__forceinline int popCallFrame();
	void addBuiltInFunction(const std::string &name,BuiltinFunctionPointer function);
private:
	std::shared_ptr<std::vector<Instruction*>> program;
	std::shared_ptr<std::vector<STATIC_DATA*>> staticsTable;
	FastStack<CallFrame*> callStack;
	NameStorage nameStorage;
	DataStack dataStack;
	VariantFactory variantFactory;
	bool terminated;
	int programCounter;
};

void StackMachine::jumpRelative(int offset)
{
	programCounter += offset;
}
void StackMachine::jumpAbsolute(int position)
{
	programCounter = position;
}
STATIC_DATA* StackMachine::getStaticData(int index)
{
	return (*staticsTable)[index];
}

DataStack* StackMachine::getDataStack()
{
	return &dataStack;
}

void StackMachine::advanceCounter()
{
	programCounter++;
}

VariantFactory* StackMachine::getVariantFactory()
{
	return &variantFactory;
}

void StackMachine::pushCallFrame(int returnAddress)
{
	callStack.push(new CallFrame(returnAddress));
}

int StackMachine::popCallFrame()
{
	CallFrame *frame = callStack.pop();
	frame->detachNames();
	int returnAddress = frame->getReturnAddress();
	delete frame;
	return returnAddress;
}

int StackMachine::getCurrentAddress()
{
	return programCounter;
}