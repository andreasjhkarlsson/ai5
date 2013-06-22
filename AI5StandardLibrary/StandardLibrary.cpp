#include "StandardLibrary.h"
#include "Macros.h"
#include "MathFunctions.h"
#include "StringFunctions.h"
#include "ListFunctions.h"
#include "TimerFunctions.h"
#include "FileFunctions.h"
#include "..\AI5Runtime\StackMachine.h"


void registerStandardLibrary(StackMachine* machine)
{


	MathFunctions::registerFunctions(machine);
	StringFunctions::registerFunctions(machine);
	ListFunctions::registerFunctions(machine);
	TimerFunctions::registerFunctions(machine);
	FileFunctions::registerFunctions(machine);


	machine->addMacro(L"autoitpid",Macros::MyPID);
}
