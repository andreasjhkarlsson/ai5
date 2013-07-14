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
	Variant* mouseDown(CallInfo* callInfo);
	Variant* mouseUp(CallInfo* callInfo);
	Variant* mouseClick(CallInfo* callInfo);
	Variant* mouseClickDrag(CallInfo* callInfo);
	Variant* mouseGetPos(CallInfo* callInfo);
	Variant* mouseWheel(CallInfo* callInfo);
	Variant* mouseGetCursor(CallInfo* callInfo);
};

