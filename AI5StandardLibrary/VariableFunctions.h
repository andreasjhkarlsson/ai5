#pragma once
#include "..\AI5Runtime\types.h"
#include "..\AI5Runtime\BinaryVariant.h"
class VariableFunctions
{
public:
	~VariableFunctions(void);
	static void registerFunctions(StackMachine* machine);
private:
	VariableFunctions(void);
	VariantReference<> stringToBinary(CallInfo* callInfo);
	VariantReference<> binaryToString(CallInfo* callInfo);
	VariantReference<> isFloat(CallInfo* callInfo);
	VariantReference<> isString(CallInfo* callInfo);
	VariantReference<> isArray(CallInfo* callInfo);
	VariantReference<> isHashMap(CallInfo* callInfo);
	VariantReference<> isBool(CallInfo* callInfo);
	VariantReference<> toString(CallInfo* callInfo);
	VariantReference<> toPointer(CallInfo* callInfo);


	void swapUtf16Endiness(wchar_t* binary,int size);
};

