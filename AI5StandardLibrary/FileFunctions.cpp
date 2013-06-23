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


Variant* FileFunctions::fileExists(Variant** args,int argsSize)
{
	validateArgCount(argsSize,1,1);

	shared_string path = args[0]->toString();


	return BooleanVariant::Get(PathFileExistsW(path->c_str()) != 0,true);

}

void FileFunctions::registerFunctions(StackMachine* machine)
{
	std::shared_ptr<FileFunctions> instance(new FileFunctions);

	machine->addBuiltInFunction(L"printline",std::bind(&printline,instance,_1,_2));
	machine->addBuiltInFunction(L"fileexists",std::bind(&fileExists,instance,_1,_2));
	machine->addBuiltInFunction(L"consolewrite",std::bind(&consoleWrite,instance,_1,_2));
	machine->addBuiltInFunction(L"consolewriteerror",std::bind(&consoleWriteError,instance,_1,_2));
	machine->addBuiltInFunction(L"consolereadline",std::bind(&consoleReadLine,instance,_1,_2));
	machine->addBuiltInFunction(L"filechangedir",std::bind(&fileChangeDir,instance,_1,_2));
}


Variant* FileFunctions::consoleWrite(Variant**args,int argSize)
{
	validateArgCount(argSize,1,1);
	std::wcout << *args[0]->toString();
	return nullptr;
}


Variant* FileFunctions::consoleWriteError(Variant** args,int argSize)
{
	validateArgCount(argSize,1,1);
	std::wcerr << *args[0]->toString();
	return nullptr;
}

Variant* FileFunctions::consoleReadLine(Variant** args,int argSize)
{
	validateArgCount(argSize,0,0);
	std::wstring str;
	std::getline(std::wcin,str);
	return new StringVariant(str);
}

Variant* FileFunctions::fileChangeDir(Variant** args,int argsSize)
{
	validateArgCount(argsSize,1,1);

	SetCurrentDirectoryW(args[0]->toString()->c_str());

	return nullptr;
}