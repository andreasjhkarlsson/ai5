#pragma once
class Variant;
class StackMachine;
class CallInfo;
class TimerFunctions
{
public:
	static void registerFunctions(StackMachine* machine);
	~TimerFunctions(void);
private:
	TimerFunctions(void);
	Variant* sleep(CallInfo* callInfo);
	Variant* timerInit(CallInfo* callInfo);
	Variant* timerDiff(CallInfo* callInfo);
	double performanceCounterFrequency; 
};

