#include "BuiltinFunctionVariant.h"
#include "StackMachine.h"

BuiltinFunctionVariant::BuiltinFunctionVariant(const std::wstring &name,BuiltinFunctionPointer func): name(name),
	func(func),FunctionVariant(NATIVE_FUNCTION)
{
}


BuiltinFunctionVariant::~BuiltinFunctionVariant(void)
{
}


void BuiltinFunctionVariant::call(StackMachine* machine,int numberOfArguments)
{
	func(machine);
	machine->advanceCounter();
}

void BuiltinFunctionVariant::print()
{
	std::wcout << "Builtin function: " << name.c_str() << std::endl;
}