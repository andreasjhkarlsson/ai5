#pragma once
#include "..\AI5Runtime\types.h"
class TimerFunctions
{
public:
	static void registerFunctions(StackMachine* machine);
	~TimerFunctions(void);
private:
	TimerFunctions(void);
	VariantReference<> sleep(CallInfo* callInfo);
	VariantReference<> timerInit(CallInfo* callInfo);
	VariantReference<> timerDiff(CallInfo* callInfo);
	double performanceCounterFrequency; 
};

