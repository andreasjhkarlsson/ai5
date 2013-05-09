#pragma once

#include <vector>
#include <string>
#include <memory>
#include "Static.h"
#include "DataStack.h"
#include "NameStorage.h"
#include "CallFrame.h"

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
	__forceinline STATIC_DATA* getStaticData(int index);
	__forceinline DataStack* getDataStack();
	__forceinline VariantFactory* getVariantFactory();
	static StackMachine* LoadFromStructuredData(const std::string& filename);
	void start();
	void terminate();
	void createLocal(int index);
	void createGlobal(int index); 
	Name* getName(int index);
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