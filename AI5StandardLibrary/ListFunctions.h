#pragma once
class Variant;
class StackMachine;
class CallInfo;
class ListFunctions
{
public:
	static void registerFunctions(StackMachine* machine);
	~ListFunctions(void);
private:
	ListFunctions(void);
	Variant* ubound(CallInfo* callInfo);
	Variant* arrayadd(CallInfo* callInfo);
};

