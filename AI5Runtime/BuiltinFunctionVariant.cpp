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
	// TODO: Make this thread safe!
	static Variant* passedArgs[MAX_ARGS];

	// Get the arguments for the function.
	for(int i=0;i<numberOfArguments;i++)
	{
		passedArgs[i] = machine->getDataStack()->get(numberOfArguments-(i+1));
	}

	// Pop of the arguments + the function object (this), which resides below the arguments.
	machine->getDataStack()->popMany(numberOfArguments+1);

	// Call the function!!1!
	Variant* result = func(passedArgs,numberOfArguments);

	// Release arguments.
	for(int i=0;i<numberOfArguments;i++)
		passedArgs[i]->release();

	machine->getDataStack()->push(result);
	machine->advanceCounter();
}

std::wostream& BuiltinFunctionVariant::format(std::wostream& stream)
{
	stream << "Builtin function: " << name.c_str();
	return stream;
}


bool BuiltinFunctionVariant::equal(Variant* other)
{
	return getType() == other->getType() &&
			(func == static_cast<BuiltinFunctionVariant*>(other)->func);
}