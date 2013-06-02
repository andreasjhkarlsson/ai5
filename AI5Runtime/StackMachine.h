#pragma once

#include <vector>
#include <memory>
#include <string>
#include <map>
#include <unordered_map>
#include <iostream>
#include "Static.h"
#include "DataStack.h"
#include "Scope.h"
#include "CallFrame.h"
#include "BuiltinFunctionVariant.h"
#include "macro.h"

class Instruction;

using std::shared_ptr;
using std::vector;

// This represents the virtual machine.
// It controls program counter, stacks, tables, scopes and memory allocation.
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
		// No callStack. This means we are at top level global scope.
		return nullptr;
	}
	__forceinline NameVariant* getNearestName(NameIdentifier identifier);
	__forceinline void setNearest(NameIdentifier identifier,Variant* variant,bool asConst=false);
	__forceinline void setLocal(NameIdentifier identifier,Variant* variant,bool asConst=false);
	__forceinline void setGlobal(NameIdentifier identifier,Variant* variant,bool asConst=false); 
	__forceinline void addNameToLocalScope(NameIdentifier identifier,NameVariant* name);
	__forceinline int popCallFrame();
	void addBuiltInFunction(const std::wstring &name,BuiltinFunctionPointer function);
	void addMacro(const std::wstring &name,MACRO_FUNCTION macroFunc);
	MACRO_FUNCTION getMacro(int staticIndex);
private:
	shared_ptr<vector<shared_ptr<Instruction>>> program;
	shared_ptr<vector<shared_ptr<StaticData>>> staticsTable;
	FastStack<CallFrame*> callStack;
	FastStack<CallFrame*> callFramePool;
	FastStack<Scope*> scopePool;
	std::unordered_map<std::wstring,MACRO_FUNCTION> macros;
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


NameVariant* StackMachine::getNearestName(NameIdentifier identifier)
{
	if(!callStack.empty())
	{
		NameVariant* name = callStack.top()->getScope()->getNameFromIndex(identifier.localId);
		if(name != nullptr)
			return name;
	}

	NameVariant* name = globalScope.getNameFromIndex(identifier.globalId);

	// If name not found from index, do a "hard" search with the name
	// Add it as an index afterwords so next lookup is FAST.
	if(name == nullptr)
	{
		std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier.staticId];
		name = globalScope.getNameFromString(*std::static_pointer_cast<StaticName>(staticData)->getName());
		// If name is still nullptr, throw error!
		globalScope.createIndexForName(this,*std::static_pointer_cast<StaticName>(staticData)->getName(),identifier.globalId);
	}

	return name;
}

// This function sets the value for a name in the nearest scope where it's found.
// If it isn't found it is added to the local scope, and if there is no local scope, to the global scope.
void StackMachine::setNearest(NameIdentifier identifier,Variant* variant,bool asConst)
{
	// Search for name in local and global scope.
	NameVariant* foundName = nullptr;
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
		foundName = targetScope->createIndexForName(this,*std::static_pointer_cast<StaticName>(staticData)->getName(),identifier.localId);
	}


	foundName->setValue(variant);

	if(asConst)
	{
		foundName->markAsConst();
	}
}


void StackMachine::setLocal(NameIdentifier identifier,Variant* variant,bool asConst)
{
	Scope* targetScope = &globalScope;
	if(!callStack.empty())
		targetScope = callStack.top()->getScope();

	NameVariant* name = targetScope->getNameFromIndex(identifier.localId);

	if(name == nullptr)
	{
		std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier.staticId];
		name = targetScope->createIndexForName(this,*std::static_pointer_cast<StaticName>(staticData)->getName(),identifier.localId);
	}

	name->setValue(variant);

	if(asConst)
	{
		name->markAsConst();
	}

}
void StackMachine::setGlobal(NameIdentifier identifier,Variant* variant,bool asConst)
{
	NameVariant* foundName = globalScope.getNameFromIndex(identifier.globalId);
	if(foundName == nullptr)
	{		
		std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier.staticId];
		foundName = globalScope.createIndexForName(this,*std::static_pointer_cast<StaticName>(staticData)->getName(),identifier.globalId);
	}
	foundName->setValue(variant);

	if(asConst)
	{
		foundName->markAsConst();
	}
}

void StackMachine::addNameToLocalScope(NameIdentifier identifier,NameVariant* name)
{
	Scope* targetScope = &globalScope;
	if(!callStack.empty())
		targetScope = callStack.top()->getScope();

	std::shared_ptr<StaticData> staticData = (*staticsTable)[identifier.staticId];
	const std::wstring& strName = *std::static_pointer_cast<StaticName>(staticData)->getName();
	targetScope->insertName(strName,identifier.localId,name);
}