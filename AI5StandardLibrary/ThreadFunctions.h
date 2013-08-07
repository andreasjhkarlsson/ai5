#pragma once
#include "..\AI5Runtime\types.h"

class ThreadFunctions
{
private:
	ThreadFunctions();
	VariantReference<> createMutex(CallInfo* callInfo);
	VariantReference<> lockMutex(CallInfo* callInfo);
	VariantReference<> unlockMutex(CallInfo* callInfo);
public:
	static void registerFunctions(StackMachine* machine);
	~ThreadFunctions(void);

};