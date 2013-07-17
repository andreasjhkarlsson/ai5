#pragma once
#include "..\AI5Runtime\types.h"
class ListFunctions
{
public:
	static void registerFunctions(StackMachine* machine);
	~ListFunctions(void);
private:
	ListFunctions(void);
	VariantReference<> ubound(CallInfo* callInfo);
	VariantReference<> arrayadd(CallInfo* callInfo);
};

