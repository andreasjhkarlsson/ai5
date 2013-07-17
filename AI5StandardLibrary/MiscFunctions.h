#pragma once
#include "..\AI5Runtime\types.h"
class MiscFunctions
{
public:
	static void registerFunctions(StackMachine* machine);
	~MiscFunctions(void);
private:
	MiscFunctions(void);
	VariantReference<> setError(CallInfo* callInfo);
	VariantReference<> setExtended(CallInfo* callInfo);
};

