#include "BuiltinFunctionVariant.h"
#include "StackMachine.h"
#include "NullVariant.h"

BuiltinFunctionVariant::BuiltinFunctionVariant(const std::wstring &name,BuiltinFunction func): name(name),
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
	// Some args are name arguments which needs to be dereferenced
	// before they're passed to the function.
	// Since they should be released and not the contained value
	// after function return we need to keep a reference to it before dereference.
	static Variant* untranslatedArgs[MAX_ARGS];

	// Get the arguments for the function.
	for(int i=0;i<numberOfArguments;i++)
	{
		passedArgs[i] = machine->getDataStack()->get(numberOfArguments-(i+1));
		untranslatedArgs[i] = passedArgs[i];
		if(passedArgs[i]->isNameType() || passedArgs[i]->isNameReferenceType())
		{
			passedArgs[i] = static_cast<NameVariant*>(passedArgs[i])->getValue();
		}
	}

	// Pop of the arguments + the function object (this), which resides below the arguments.
	machine->getDataStack()->popMany(numberOfArguments+1);

	// Call the function!!1!
	Variant* result = func(passedArgs,numberOfArguments);
	if(result == nullptr)
	{
		result = &NullVariant::Instance;
		result->addRef();
	}


	// Release arguments.
	for(int i=0;i<numberOfArguments;i++)
		untranslatedArgs[i]->release();

	machine->getDataStack()->push(result);
	machine->advanceCounter();
}

std::wostream& BuiltinFunctionVariant::format(std::wostream& stream) const
{
	stream << "Builtin function: " << name.c_str();
	return stream;
}


bool BuiltinFunctionVariant::equal(Variant* other)
{
	return getType() == other->getType() &&
			(name == static_cast<BuiltinFunctionVariant*>(other)->name);
}