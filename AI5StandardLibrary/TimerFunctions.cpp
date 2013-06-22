#include "TimerFunctions.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\NullVariant.h"
#include <Windows.h>
#include <functional>
#include <memory>
using namespace std::placeholders;

TimerFunctions::TimerFunctions(void)
{
}


TimerFunctions::~TimerFunctions(void)
{
}


Variant* TimerFunctions::sleep(Variant** args,int argsSize)
{
	Variant* var = args[0];

	int sleepDelay = var->toInteger32();

	Sleep(sleepDelay);
	NullVariant::Instance.addRef();
	return &NullVariant::Instance;
}



void TimerFunctions::registerFunctions(StackMachine* machine)
{
	std::shared_ptr<TimerFunctions> instance(new TimerFunctions);

	machine->addBuiltInFunction(L"sleep",std::bind(&sleep,instance,_1,_2));
}