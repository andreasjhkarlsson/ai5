#include "TimerFunctions.h"
#include "..\AI5Runtime\StackMachineThread.h"
#include "..\AI5Runtime\NullVariant.h"
#include "..\AI5Runtime\CallInfo.h"
#include <Windows.h>
#include <functional>
#include <memory>
using namespace std::placeholders;

TimerFunctions::TimerFunctions(void)
{
	// Query performance frequency to be used by timerDiff function.
    LARGE_INTEGER li;
    QueryPerformanceFrequency(&li);
	performanceCounterFrequency = double(li.QuadPart)/1000.0;
}


TimerFunctions::~TimerFunctions(void)
{
}


Variant* TimerFunctions::sleep(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);

	int sleepDelay = callInfo->getInt32Arg(0);

	Sleep(sleepDelay);
	
	return nullptr;
}



void TimerFunctions::registerFunctions(StackMachineThread* machine)
{
	std::shared_ptr<TimerFunctions> instance(new TimerFunctions);

	machine->addBuiltInFunction(L"sleep",std::bind(&sleep,instance,_1));
	machine->addBuiltInFunction(L"timerinit",std::bind(&timerInit,instance,_1));
	machine->addBuiltInFunction(L"timerdiff",std::bind(&timerDiff,instance,_1));
}


Variant* TimerFunctions::timerInit(CallInfo* callInfo)
{
	callInfo->validateArgCount(0,0);
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return new Integer64Variant(li.QuadPart);
}

Variant* TimerFunctions::timerDiff(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	__int64 start = callInfo->getInt64Arg(0);
	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);

	return new FloatingVariant((end.QuadPart-start)/performanceCounterFrequency);
}

