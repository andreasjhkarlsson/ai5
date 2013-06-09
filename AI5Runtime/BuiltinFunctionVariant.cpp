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

std::wostream& BuiltinFunctionVariant::format(std::wostream& stream)
{
	stream << "Builtin function: " << name.c_str();
	return stream;
}