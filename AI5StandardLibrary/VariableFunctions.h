#pragma once
#include "..\AI5Runtime\BinaryVariant.h"
class Variant;
class StackMachineThread;
class CallInfo;
class VariableFunctions
{
public:
	~VariableFunctions(void);
	static void registerFunctions(StackMachineThread* machine);
private:
	VariableFunctions(void);
	Variant* stringToBinary(CallInfo* callInfo);
	Variant* binaryToString(CallInfo* callInfo);
	Variant* isFloat(CallInfo* callInfo);
	Variant* isString(CallInfo* callInfo);
	Variant* isArray(CallInfo* callInfo);
	Variant* isHashMap(CallInfo* callInfo);
	Variant* isBool(CallInfo* callInfo);
	Variant* ToString(CallInfo* callInfo);


	void swapUtf16Endiness(wchar_t* binary,int size);
};

