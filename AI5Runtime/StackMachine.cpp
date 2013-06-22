#include <map>
#include "StackMachine.h"
#include "RuntimeError.h"
#include "NullVariant.h"
#include "Instruction.h"
#include "..\AI5StandardLibrary\StandardLibrary.h"

StackMachine::StackMachine(shared_ptr<vector<shared_ptr<StaticData>>> statics,
	shared_ptr<vector<shared_ptr<Instruction>>> program): programCounter(0),
	dataStack(DATA_STACK_SIZE),staticsTable(statics),program(program), blockStack(BLOCK_STACK_SIZE), 
	currentCallBlock(nullptr), verbose(false), globalScope(new Scope())
{
	registerStandardLibrary(this);
}

StackMachine::~StackMachine(void)
{
	delete globalScope;
}

int StackMachine::start()
{
	terminated = false;
	int returnCode;
	try
	{
		while (!terminated) 
		{
			if(verbose)
			{
				std::wcout << "\t";
				(*program)[programCounter]->format(std::wcout,this);
				std::wcout << std::endl;
			}
			
			(*program)[programCounter]->execute(this);		
		}
		returnCode = dataStack.top()->toInteger32();
	}
	catch(const RuntimeError& error)
	{
		std::wcout << L"Runtime error:" << std::endl << error.getMessage() <<
			std::endl << "The program will now terminate." << std::endl;
		returnCode = -1;
	}

	return returnCode;
}

void StackMachine::terminate()
{
	terminated = true;
}

void StackMachine::addBuiltInFunction(const std::wstring &name,BuiltinFunction function)
{
	globalScope->createName(this,name)->setValue(new BuiltinFunctionVariant(name,function));
}

void StackMachine::addMacro(const std::wstring &name,MACRO_FUNCTION macroFunc)
{
	macros[name] = macroFunc;
}

MACRO_FUNCTION StackMachine::getMacro(int staticIndex)
{
	return macros[*std::static_pointer_cast<StaticMacro>((*staticsTable)[staticIndex])->getName()];
}


void StackMachine::disassemble()
{
	for(int i=0;i<program->size();i++)
	{
		std::wcout << i << ": ";
		(*program)[i]->format(std::wcout,this);
		std::wcout << std::endl;
	}
}

void StackMachine::setVerbose()
{
	verbose = true;
}