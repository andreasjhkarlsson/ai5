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
	static StackMachine* LoadFromStructuredData(const std::wstring& filename);
	void start();
	void terminate();
	__forceinline void pushCallFrame(int returnAddress);
	__forceinline CallFrame* getCurrentCallFrame()
	{
		if(callStack.size() > 0)
			return callStack.top();
		return nullptr;
	}
	__forceinline Variant* getNearest(NameIdentifier identifier);
	__forceinline void setNearest(NameIdentifier identifier,Variant* variant);
	__forceinline void setLocal(NameIdentifier identifier,Variant* variant);
	__forceinline void setGlobal(NameIdentifier identifier,Variant* variant); 
	__forceinline int popCallFrame();
	void addBuiltInFunction(const std::wstring &name,BuiltinFunctionPointer function);
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


Variant* StackMachine::getNearest(NameIdentifier identifier)
{
	if(!callStack.empty())
	{
		Name* name = callStack.top()->getScope()->getNameFromIndex(identifier.localId);
		if(name != nullptr)
			return name->get();
	}

	Name* name = globalScope.getNameFromIndex(identifier.globalId);

	// If name not found from index, do a "hard" search with the name
	// Add it as an index afterwords so next lookup is FAST.
	if(name == nullptr)
	{
		std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier.staticId];
		name = globalScope.getNameFromString(*std::static_pointer_cast<StaticName>(staticData)->getName());
		// If name is still nullptr, throw error!
		globalScope.createIndexForName(*std::static_pointer_cast<StaticName>(staticData)->getName(),identifier.globalId);
	}

	return name->get();
}

// This function sets the value for a name in the nearest scope where it's found.
// If it isn't found it is added to the local scope, and if there is no local scope, to the global scope.
void StackMachine::setNearest(NameIdentifier identifier,Variant* variant)
{
	// Search for name in local and global scope.
	Name* foundName = nullptr;
	if(!callStack.empty())
		foundName = callStack.top()->getScope()->getNameFromIndex(identifier.localId);
	if(foundName == nullptr)
		foundName = globalScope.getNameFromIndex(identifier.globalId);

	// If not found, add it as a new name to the nearest scope.
	if(foundName == nullptr)
	{
		Scope* targetScope = nullptr;
		if(!callStack.empty())
			targetScope = callStack.top()->getScope();
		else
			targetScope = &globalScope;
		std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier.staticId];

		// The name may be defined without this index. This doesn't matter as the createIndexForName will check
		// if the name is already defined.
		foundName = targetScope->createIndexForName(*std::static_pointer_cast<StaticName>(staticData)->getName(),identifier.localId);
	}


	foundName->set(variant);
}


void StackMachine::setLocal(NameIdentifier identifier,Variant* variant)
{
	Scope* targetScope = &globalScope;
	if(!callStack.empty())
		targetScope = callStack.top()->getScope();

	Name* name = targetScope->getNameFromIndex(identifier.localId);

	if(name == nullptr)
	{
		std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier.staticId];
		name = targetScope->createIndexForName(*std::static_pointer_cast<StaticName>(staticData)->getName(),identifier.localId);
	}

	name->set(variant);


}
void StackMachine::setGlobal(NameIdentifier identifier,Variant* variant)
{
	Name* foundName = globalScope.getNameFromIndex(identifier.globalId);
	if(foundName == nullptr)
	{		
		std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier.staticId];
		foundName = globalScope.createIndexForName(*std::static_pointer_cast<StaticName>(staticData)->getName(),identifier.globalId);
	}
	foundName->set(variant);
}