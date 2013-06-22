#include "Macros.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\Integer64Variant.h"
#include <Windows.h>

Variant* Macros::MyPID(StackMachine*)
{
	return new Integer64Variant(GetCurrentProcessId());
}