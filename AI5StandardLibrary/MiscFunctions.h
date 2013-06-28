#pragma once
class StackMachine;
class CallInfo;
class Variant;
class MiscFunctions
{
public:
	static void registerFunctions(StackMachine* machine);
	~MiscFunctions(void);
private:
	MiscFunctions(void);
	Variant* setError(CallInfo* callInfo);
	Variant* setExtended(CallInfo* callInfo);
};

