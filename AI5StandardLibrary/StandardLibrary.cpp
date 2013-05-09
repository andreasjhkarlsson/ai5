#include "..\AI5Runtime\StackMachine.h"
#include "StandardLibrary.h"
#include "functions.h"

namespace AI5StandardLibrary
{
	void registerFunctions(StackMachine* machine)
	{
		machine->addBuiltInFunction("hello",hello);
		machine->addBuiltInFunction("sleep",sleep);

	}
}