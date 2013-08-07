#pragma once
#include "..\AI5Runtime\types.h"
class Macros
{
public:
	static void registerMacros(StackMachine*);
private:
	static VariantReference<> MyPID(StackMachineThread*);
	static VariantReference<> CRLF(StackMachineThread*);
	static VariantReference<> CR(StackMachineThread*);
	static VariantReference<> LF(StackMachineThread*);
	static VariantReference<> TAB(StackMachineThread*);
	static VariantReference<> Username(StackMachineThread*);
	static VariantReference<> WorkingDirectory(StackMachineThread*);
	static VariantReference<> TempDirectory(StackMachineThread*);
	static VariantReference<> SystemDirectory(StackMachineThread*);
	static VariantReference<> WindowsDirectory(StackMachineThread*);
	static VariantReference<> millisecond(StackMachineThread*);
	static VariantReference<> second(StackMachineThread*);
	static VariantReference<> minute(StackMachineThread*);
	static VariantReference<> hour(StackMachineThread*);
	static VariantReference<> monthDay(StackMachineThread*);
	static VariantReference<> month(StackMachineThread*);
	static VariantReference<> year(StackMachineThread*);
	static VariantReference<> weekDay(StackMachineThread*);
	static VariantReference<> errorCode(StackMachineThread*);
	static VariantReference<> extendedCode(StackMachineThread*);
	static VariantReference<> threadId(StackMachineThread*);
	static VariantReference<> osVersion(StackMachineThread*);
};

