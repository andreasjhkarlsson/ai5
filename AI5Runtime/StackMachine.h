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
	__forceinline CallFrame* getCurrentCallFrame()
	{
		if(callStack.size() > 0)
			return callStack.top();
		return nullptr;
	}
	__forceinline Variant* getNearest(int identifier);
	__forceinline void setNearest(int identifier,Variant* variant);
	__forceinline void setLocal(int identifier,Variant* variant);
	__forceinline void setGlobal(int identifier,Variant* variant); 
	__forceinline int popCallFrame();
	void addBuiltInFunction(const std::string &name,BuiltinFunctionPointer function);
private:
	shared_ptr<vector<shared_ptr<Instruction>>> program;
	shared_ptr<vector<shared_ptr<StaticData>>> staticsTable;
	FastStack<CallFrame*> callStack;
	FastStack<CallFrame*> callFramePool;
	FastStack<Scope*> scopePool;
	Scope globalScope;
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
	frame->setScope(scopePool.pop());
	frame->setReturnAddress(returnAddress);
	callStack.push(frame);
}

int StackMachine::popCallFrame()
{
	CallFrame *frame = callStack.pop();
	
	frame->getScope()->reset();
	scopePool.push(frame->getScope());

	int returnAddress = frame->getReturnAddress();
	callFramePool.push(frame);
	return returnAddress;
}

int StackMachine::getCurrentAddress()
{
	return programCounter;
}


Variant* StackMachine::getNearest(int identifier)
{
	if(!callStack.empty())
	{
		Name* name = callStack.top()->getScope()->getNameFromIndex(identifier);
		if(name != nullptr)
			return name->get();
	}

	Name* name = globalScope.getNameFromIndex(identifier);

	// If name not found from index, do a "hard" search with the name
	// Add it as an index afterwords so next lookup is FAST.
	if(name == nullptr)
	{
		std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier];
		name = globalScope.getNameFromString(std::static_pointer_cast<StaticName>(staticData)->getName());
		// If name is still nullptr, throw error!
		globalScope.createIndexForName(std::static_pointer_cast<StaticName>(staticData)->getName(),identifier);
	}

	return name->get();
}
void StackMachine::setNearest(int identifier,Variant* variant)
{

	Name* foundName = nullptr;
	if(!callStack.empty())
		foundName = callStack.top()->getScope()->getNameFromIndex(identifier);
	if(foundName == nullptr)
		foundName = globalScope.getNameFromIndex(identifier);

	if(foundName == nullptr)
	{
		Scope* targetScope = nullptr;
		if(!callStack.empty())
			targetScope = callStack.top()->getScope();
		else
			targetScope = &globalScope;
		std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier];
		foundName = targetScope->createIndexForName(std::static_pointer_cast<StaticName>(staticData)->getName(),identifier);
	}


	foundName->set(variant);
}


void StackMachine::setLocal(int identifier,Variant* variant)
{
	// This function is the same thing as setNearest.
	Scope* targetScope = &globalScope;
	if(!callStack.empty())
		targetScope = callStack.top()->getScope();

	Name* name = targetScope->getNameFromIndex(identifier);

	if(name == nullptr)
	{
		std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier];
		name = targetScope->createIndexForName(std::static_pointer_cast<StaticName>(staticData)->getName(),identifier);
	}

	name->set(variant);


}
void StackMachine::setGlobal(int identifier,Variant* variant)
{
	Name* foundName = globalScope.getNameFromIndex(identifier);
	if(foundName == nullptr)
	{		
		std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier];
		foundName = globalScope.createIndexForName(std::static_pointer_cast<StaticName>(staticData)->getName(),identifier);
	}
	foundName->set(variant);
}