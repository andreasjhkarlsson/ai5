#include "BuiltinFunctionVariant.h"
#include "StackMachineThread.h"
#include "NullVariant.h"
#include "CallInfo.h"

BuiltinFunctionVariant::BuiltinFunctionVariant(const UnicodeString &name,BuiltinFunction func): name(name),
	func(func),FunctionVariant(TYPE)
{
}


BuiltinFunctionVariant::~BuiltinFunctionVariant(void)
{
}


void BuiltinFunctionVariant::call(StackMachineThread* machine,int numberOfArguments)
{
	CallInfo callInfo(machine);

	// Get the arguments for the function.
	for(int i=0;i<numberOfArguments;i++)
	{
		Variant* arg = machine->getDataStack()->get(numberOfArguments-(i+1));
		
		callInfo.addArg(arg);
	}

	// Pop of the arguments + the function object (this), which resides below the arguments.
	machine->getDataStack()->popMany(numberOfArguments+1);

	// Call the function!!1!
	Variant* result = func(&callInfo);
	if(result == nullptr)
	{
		result = &NullVariant::Instance;
		result->addRef();
	}

	machine->getDataStack()->push(result);
	machine->advanceCounter();
}

std::wostream& BuiltinFunctionVariant::format(std::wostream& stream) const
{
	stream << "Builtin function: ";// << name.getTerminatedBuffer();
	return stream;
}


bool BuiltinFunctionVariant::equal(Variant* other)
{
	return getType() == other->getType() &&
			(name == static_cast<BuiltinFunctionVariant*>(other)->name);
}