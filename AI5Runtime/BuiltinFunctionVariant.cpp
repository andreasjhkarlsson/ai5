#include "BuiltinFunctionVariant.h"
#include "StackMachine.h"

BuiltinFunctionVariant::BuiltinFunctionVariant(const std::string &name,BuiltinFunctionPointer func): name(name),
	func(func),FunctionVariant(NATIVE_FUNCTION_VAR)
{
}


BuiltinFunctionVariant::~BuiltinFunctionVariant(void)
{
}


void BuiltinFunctionVariant::call(StackMachine* machine)
{
	func(machine);
	machine->advanceCounter();
}

void BuiltinFunctionVariant::print()
{
	std::cout << "Builtin function: " << name << std::endl;
}