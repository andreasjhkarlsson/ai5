#include "StackMachine.h"
#include <map>
#include "NullVariant.h"
#include "Instruction.h"
#include "..\AI5StandardLibrary\StandardLibrary.h"

StackMachine::StackMachine(std::shared_ptr<std::vector<STATIC_DATA*>> statics,
	std::shared_ptr<std::vector<Instruction*>> program): programCounter(0),
	dataStack(64*1024),staticsTable(statics),program(program), callStack(128)
{
	AI5StandardLibrary::registerFunctions(this);
}

StackMachine::~StackMachine(void)
{
}

void StackMachine::start()
{
	terminated = false;

	while (!terminated) 
	{
	//	std::cout << "Executing instruction @ " << programCounter << std::endl;
		(*program)[programCounter]->execute(this);
	}
}

void StackMachine::terminate()
{
	terminated = true;
}

StackMachine* StackMachine::LoadFromStructuredData(const std::string& filename)
{
	return nullptr;
}


void StackMachine::addBuiltInFunction(const std::string &name,BuiltinFunctionPointer function)
{
	nameStorage.createName(name)->setBuiltin(new BuiltinFunctionVariant(name,function));
}