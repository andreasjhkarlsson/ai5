#include "DialogFunctions.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\Integer32Variant.h"
#include "..\AI5Runtime\CallInfo.h"
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

VariantReference<> DialogFunctions::msgbox(CallInfo* callInfo)
{
	callInfo->validateArgCount(3,5);

	int flag = callInfo->getInt32Arg(0);
	shared_string title = callInfo->getStringArg(1);
	shared_string text = callInfo->getStringArg(2);

	return MessageBoxW(NULL,text->getTerminatedBuffer(),title->getTerminatedBuffer(),flag);
}


void DialogFunctions::registerFunctions(StackMachine* machine)
{
	std::shared_ptr<DialogFunctions> instance(new DialogFunctions);

	machine->addBuiltInFunction(L"msgbox",std::bind(&msgbox,instance,_1));
}
