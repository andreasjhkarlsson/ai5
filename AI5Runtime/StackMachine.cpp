#include "StackMachine.h"
#include <map>
#include "NullVariant.h"
#include "Instruction.h"
#include "..\AI5StandardLibrary\StandardLibrary.h"

StackMachine::StackMachine(shared_ptr<vector<shared_ptr<StaticData>>> statics,
	shared_ptr<vector<shared_ptr<Instruction>>> program): programCounter(0),
	dataStack(64*1024),staticsTable(statics),program(program), callStack(128), callFramePool(128),scopePool(128)
{
	AI5StandardLibrary::registerFunctions(this);

	for(int i=0;i<128;i++)
	{
		callFramePool.push(new CallFrame());
	}

	for(int i=0;i<128;i++)
	{
		scopePool.push(new Scope());
	}
}

StackMachine::~StackMachine(void)
{
}

void StackMachine::start()
{
	terminated = false;

	while (!terminated) 
	{
		#ifdef _DEBUG
			std::cout << "\t";
			program->operator[](programCounter)->format(std::cout,this);
			std::cout << std::endl;
		#endif
			
		program->operator[](programCounter)->execute(this);
		
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
	globalScope.createName(name)->set(new BuiltinFunctionVariant(name,function));
}