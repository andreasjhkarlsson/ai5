#include "..\AI5Runtime\StackMachine.h"
#include "StandardLibrary.h"
#include "functions.h"

namespace AI5StandardLibrary
{
	void registerFunctions(StackMachine* machine)
	{
		machine->addBuiltInFunction(L"hello",hello);
		machine->addBuiltInFunction(L"sleep",sleep);
		machine->addBuiltInFunction(L"printline",printline);
		machine->addBuiltInFunction(L"getint",getint);
	}
}