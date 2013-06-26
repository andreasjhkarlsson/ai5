#include "DialogFunctions.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\Integer32Variant.h"
#include <cmath>
#include <functional>
#include <memory>
#include <Windows.h>
using namespace std::placeholders;

DialogFunctions::DialogFunctions(void)
{
}


DialogFunctions::~DialogFunctions(void)
{
}

Variant* DialogFunctions::msgbox(Variant** args,int argCount)
{
	validateArgCount(argCount,3,5);

	int flag = args[0]->toInteger32();
	shared_string title = args[1]->toString();
	shared_string text = args[2]->toString();

	return new Integer32Variant(MessageBoxW(NULL,text->c_str(),title->c_str(),flag));
}


void DialogFunctions::registerFunctions(StackMachine* machine)
{
	std::shared_ptr<DialogFunctions> instance(new DialogFunctions);

	machine->addBuiltInFunction(L"msgbox",std::bind(&msgbox,instance,_1,_2));
}
