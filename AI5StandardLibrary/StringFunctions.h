#pragma once

#include "..\AI5Runtime\types.h"

class StringFunctions
{
public:
	static void registerFunctions(StackMachine* machine);
	~StringFunctions(void);
private:
	StringFunctions(void);
	VariantReference<> stringLower(CallInfo* callInfo);
	VariantReference<> stringUpper(CallInfo* callInfo);
	VariantReference<> stringLen(CallInfo* callInfo);
	VariantReference<> stringLeft(CallInfo* callInfo);
	VariantReference<> stringTrimLeft(CallInfo* callInfo);
	VariantReference<> stringRight(CallInfo* callInfo);
	VariantReference<> stringTrimRight(CallInfo* callInfo);
	VariantReference<> stringIsDigit(CallInfo* callInfo);
};

