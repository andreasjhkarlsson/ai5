#pragma once

#include "..\AI5Runtime\types.h"

class DialogFunctions
{
public:
	static void registerFunctions(StackMachine* machine);
	~DialogFunctions(void);
private:
	DialogFunctions(void);
	VariantReference<> msgbox(CallInfo* callInfo);
};

