#pragma once


class Variant;
class StackMachineThread;
class CallInfo;

class StringFunctions
{
public:
	static void registerFunctions(StackMachineThread* machine);
	~StringFunctions(void);
private:
	StringFunctions(void);
	Variant* stringLower(CallInfo* callInfo);
	Variant* stringUpper(CallInfo* callInfo);
	Variant* stringLen(CallInfo* callInfo);
	Variant* stringLeft(CallInfo* callInfo);
	Variant* stringTrimLeft(CallInfo* callInfo);
	Variant* stringRight(CallInfo* callInfo);
	Variant* stringTrimRight(CallInfo* callInfo);
	Variant* stringIsDigit(CallInfo* callInfo);
};

