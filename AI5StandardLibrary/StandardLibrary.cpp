#include "StandardLibrary.h"
#include "Macros.h"
#include "MathFunctions.h"
#include "StringFunctions.h"
#include "ListFunctions.h"
#include "TimerFunctions.h"
#include "FileFunctions.h"
#include "DialogFunctions.h"
#include "VariableFunctions.h"
#include "GraphicsAndSoundFunctions.h"
#include "DllCallFunctions.h"
#include "MiscFunctions.h"
#include "MouseFunctions.h"
#include "SystemFunctions.h"
#include "..\AI5Runtime\StackMachine.h"
#include <Windows.h>
#include "autoit3.h"


void registerStandardLibrary(StackMachine* machine)
{
	AU3_Init();
	MathFunctions::registerFunctions(machine);
	StringFunctions::registerFunctions(machine);
	ListFunctions::registerFunctions(machine);
	TimerFunctions::registerFunctions(machine);
	FileFunctions::registerFunctions(machine);
	DialogFunctions::registerFunctions(machine);
	VariableFunctions::registerFunctions(machine);
	GraphicsAndSoundFunctions::registerFunctions(machine);
	DllCallFunctions::registerFunctions(machine);
	MiscFunctions::registerFunctions(machine);
	MouseFunctions::registerFunctions(machine);
	SystemFunctions::registerFunctions(machine);
	Macros::registerMacros(machine);
}
