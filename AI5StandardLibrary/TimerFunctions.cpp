#include "TimerFunctions.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\NullVariant.h"
#include <Windows.h>
#include <functional>
#include <memory>
using namespace std::placeholders;

TimerFunctions::TimerFunctions(void)
{
	// Query performance frqeuwncy to be used by timerDiff function.
    LARGE_INTEGER li;
    QueryPerformanceFrequency(&li);
	performanceCounterFrequency = double(li.QuadPart)/1000.0;
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
	machine->addBuiltInFunction(L"timerinit",std::bind(&timerInit,instance,_1,_2));
	machine->addBuiltInFunction(L"timerdiff",std::bind(&timerDiff,instance,_1,_2));
}


Variant* TimerFunctions::timerInit(Variant** args,int argsSize)
{
	validateArgCount(argsSize,0,0);
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return new Integer64Variant(li.QuadPart);
}

Variant* TimerFunctions::timerDiff(Variant** args,int argsSize)
{
	validateArgCount(argsSize,1,1);
	__int64 start = args[0]->toInteger64();
	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);

	return new FloatingVariant((end.QuadPart-start)/performanceCounterFrequency);
}

