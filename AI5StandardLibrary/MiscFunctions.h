#pragma once
class StackMachineThread;
class CallInfo;
class Variant;
class MiscFunctions
{
public:
	static void registerFunctions(StackMachineThread* machine);
	~MiscFunctions(void);
private:
	MiscFunctions(void);
	Variant* setError(CallInfo* callInfo);
	Variant* setExtended(CallInfo* callInfo);
};

