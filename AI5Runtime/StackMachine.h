#pragma once

#include <vector>
#include <memory>
#include <string>
#include <map>
#include <iostream>
#include "Static.h"
#include "DataStack.h"
#include "NameStorage.h"
#include "CallFrame.h"
#include "BuiltinFunctionVariant.h"

class Instruction;

using std::shared_ptr;
using std::vector;

class StackMachine
{
public:
	StackMachine(shared_ptr<vector<shared_ptr<StaticData>>> statics,
					shared_ptr<vector<shared_ptr<Instruction>>> program);
	~StackMachine(void);
	// These methods are called by instructions, so they need
	// to be inlined for maximum speed.
	__forceinline void advanceCounter();
	__forceinline void jumpRelative(int offset);
	__forceinline void jumpAbsolute(int position);
	__forceinline int getCurrentAddress();
	__forceinline StaticData* getStaticData(int index);
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
	__forceinline CallFrame* getCurrentCallFrame()
	{
		if(callStack.size() > 0)
			return callStack.top();
		return nullptr;
	}
	__forceinline int popCallFrame();
	void addBuiltInFunction(const std::string &name,BuiltinFunctionPointer function);
private:
	shared_ptr<vector<shared_ptr<Instruction>>> program;
	shared_ptr<vector<shared_ptr<StaticData>>> staticsTable;
	FastStack<CallFrame*> callStack;
	FastStack<CallFrame*> callFramePool;
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
StaticData* StackMachine::getStaticData(int index)
{
	return (*staticsTable)[index].get();
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
	CallFrame* frame = callFramePool.pop();
	frame->setReturnAddress(returnAddress);
	callStack.push(frame);
}

int StackMachine::popCallFrame()
{
	CallFrame *frame = callStack.pop();
	frame->detachNames();
	int returnAddress = frame->getReturnAddress();
	callFramePool.push(frame);
	return returnAddress;
}

int StackMachine::getCurrentAddress()
{
	return programCounter;
}

