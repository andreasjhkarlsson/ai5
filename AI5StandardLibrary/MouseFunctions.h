#pragma once
class Variant;
class StackMachine;
class CallInfo;
class MouseFunctions
{
public:
	static void registerFunctions(StackMachine* machine);
	~MouseFunctions(void);
private:
	MouseFunctions(void);
	Variant* mouseMove(CallInfo* callInfo);
};