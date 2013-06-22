#include "FileFunctions.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\NullVariant.h"
#include "..\AI5Runtime\BooleanVariant.h"
#include <Windows.h>
#include <Shlwapi.h>
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

	for(int i=0;i<argsSize;i++)
	{
		if(i > 0)
			std::wcout << ", ";
		Variant* var = args[i];
		var->format(std::wcout);
		
	}

	std::wcout << std::endl;

	NullVariant::Instance.addRef();
	return &NullVariant::Instance;
}


Variant* FileFunctions::fileexists(Variant** args,int argsSize)
{
	validateArgCount(argsSize,1,1);

	shared_string path = args[0]->toString();


	return BooleanVariant::Get(PathFileExistsW(path->c_str()),true);

}

void FileFunctions::registerFunctions(StackMachine* machine)
{
	std::shared_ptr<FileFunctions> instance(new FileFunctions);

	machine->addBuiltInFunction(L"printline",std::bind(&printline,instance,_1,_2));
	machine->addBuiltInFunction(L"fileexists",std::bind(&fileexists,instance,_1,_2));
}