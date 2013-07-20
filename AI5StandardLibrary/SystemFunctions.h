#pragma once
#include "..\AI5Runtime\types.h"

class SystemFunctions
{
private:
	SystemFunctions(StackMachine*);
	VariantReference<> runGC(CallInfo* callInfo);
	VariantReference<> startThread(CallInfo* callInfo);
	VariantReference<> joinThread(CallInfo* callInfo);
	StackMachine* machine;
public:
	static void registerFunctions(StackMachine* machine);
	~SystemFunctions(void);

};

