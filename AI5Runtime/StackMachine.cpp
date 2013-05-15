#include "StackMachine.h"
#include <map>
#include "NullVariant.h"
#include "Instruction.h"
#include "..\AI5StandardLibrary\StandardLibrary.h"

StackMachine::StackMachine(shared_ptr<vector<shared_ptr<StaticData>>> statics,
	shared_ptr<vector<shared_ptr<Instruction>>> program): programCounter(0),
	dataStack(64*1024),staticsTable(statics),program(program), callStack(128), callFramePool(128)
{
	AI5StandardLibrary::registerFunctions(this);

	for(int i=0;i<128;i++)
	{
		callFramePool.push(new CallFrame());
	}
	
	for(size_t index = 0;index < statics->size(); index++)
	{
		unsigned char t = statics->operator[](index)->getType();
		if(t == StaticData::NAME)
		{
			
			nameStorage.createIndexForName(std::static_pointer_cast<StaticName>(statics->operator[](index))->getName(),index);			;
		}
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
	//	std::cout << "Executing instruction @ " << programCounter << std::endl;
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
	nameStorage.createName(name)->setBuiltin(new BuiltinFunctionVariant(name,function));
}