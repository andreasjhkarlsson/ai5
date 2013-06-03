#include <map>
#include "StackMachine.h"
#include "RuntimeError.h"
#include "NullVariant.h"
#include "Instruction.h"
#include "..\AI5StandardLibrary\StandardLibrary.h"

StackMachine::StackMachine(shared_ptr<vector<shared_ptr<StaticData>>> statics,
	shared_ptr<vector<shared_ptr<Instruction>>> program): programCounter(0),
	dataStack(DATA_STACK_SIZE),staticsTable(statics),program(program), callStack(CALL_STACK_SIZE), 
	callFramePool(CALL_FRAME_POOL_SIZE),scopePool(SCOPE_POOL_SIZE)
{
	AI5StandardLibrary::registerFunctions(this);

	for(int i=0;i<CALL_FRAME_POOL_SIZE;i++)
	{
		callFramePool.push(new CallFrame());
	}

	for(int i=0;i<SCOPE_POOL_SIZE;i++)
	{
		scopePool.push(new Scope());
	}
}

StackMachine::~StackMachine(void)
{
}

int StackMachine::start()
{
	terminated = false;

	try
	{
		while (!terminated) 
		{
			#ifdef _DEBUG
				std::wcout << "\t";
				program->operator[](programCounter)->format(std::wcout,this);
				std::wcout << std::endl;
			#endif
			
			program->operator[](programCounter)->execute(this);
		
		}
		return dataStack.top()->toInteger32();
	} catch(const RuntimeError& error)
	{
		std::wcout << L"Runtime error:" << std::endl << error.getMessage() <<
			std::endl << "The program will now terminate." << std::endl;
		return -1;
	}
}

void StackMachine::terminate()
{
	terminated = true;
}

StackMachine* StackMachine::LoadFromStructuredData(const std::wstring& filename)
{
	return nullptr;
}


void StackMachine::addBuiltInFunction(const std::wstring &name,BuiltinFunctionPointer function)
{
	globalScope.createName(this,name)->setValue(new BuiltinFunctionVariant(name,function));
}

void StackMachine::addMacro(const std::wstring &name,MACRO_FUNCTION macroFunc)
{
	macros[name] = macroFunc;
}

MACRO_FUNCTION StackMachine::getMacro(int staticIndex)
{
	return macros[*std::static_pointer_cast<StaticMacro>((*staticsTable)[staticIndex])->getName()];
}
