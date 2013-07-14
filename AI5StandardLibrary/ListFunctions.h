#pragma once
class Variant;
class StackMachineThread;
class CallInfo;
class ListFunctions
{
public:
	static void registerFunctions(StackMachineThread* machine);
	~ListFunctions(void);
private:
	ListFunctions(void);
	Variant* ubound(CallInfo* callInfo);
	Variant* arrayadd(CallInfo* callInfo);
};

