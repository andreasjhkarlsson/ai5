#pragma once
#include "functions.h"
class Variant;
class StackMachine;
class TimerFunctions :
	public Functions
{
public:
	static void registerFunctions(StackMachine* machine);
	~TimerFunctions(void);
private:
	TimerFunctions(void);
	Variant* sleep(Variant** args,int argsSize);
};

