#pragma once
class Variant;
class StackMachineThread;
class CallInfo;
class TimerFunctions
{
public:
	static void registerFunctions(StackMachineThread* machine);
	~TimerFunctions(void);
private:
	TimerFunctions(void);
	Variant* sleep(CallInfo* callInfo);
	Variant* timerInit(CallInfo* callInfo);
	Variant* timerDiff(CallInfo* callInfo);
	double performanceCounterFrequency; 
};

