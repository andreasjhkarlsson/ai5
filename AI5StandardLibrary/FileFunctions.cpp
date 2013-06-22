#include "FileFunctions.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\NullVariant.h"
#include <Windows.h>
#include <functional>
#include <memory>
using namespace std::placeholders;

FileFunctions::FileFunctions(void)
{
}


FileFunctions::~FileFunctions(void)
{
}


Variant* FileFunctions::printline(Variant** args,int argsSize)
{
	Variant* var = args[0];

	var->format(std::wcout);
	std::wcout << std::endl;

	NullVariant::Instance.addRef();
	return &NullVariant::Instance;
}


void FileFunctions::registerFunctions(StackMachine* machine)
{
	std::shared_ptr<FileFunctions> instance(new FileFunctions);

	machine->addBuiltInFunction(L"printline",std::bind(&printline,instance,_1,_2));
}