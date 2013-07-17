#pragma once

#include "..\AI5Runtime\types.h"

class DllCallFunctions
{
public:
	static void registerFunctions(StackMachine* machine);
	~DllCallFunctions();
private:
	DllCallFunctions();
	VariantReference<> dllcall(CallInfo* callInfo);
	VariantReference<> dllcalladdress(CallInfo* callInfo);
	VariantReference<> dllopen(CallInfo* callInfo);
	VariantReference<> dllclose(CallInfo* callInfo);
};

