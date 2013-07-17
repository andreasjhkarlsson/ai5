#pragma once
#include "..\AI5Runtime\types.h"
class MouseFunctions
{
public:
	static void registerFunctions(StackMachine* machine);
	~MouseFunctions(void);
private:
	MouseFunctions(void);
	VariantReference<> mouseMove(CallInfo* callInfo);
	VariantReference<> mouseDown(CallInfo* callInfo);
	VariantReference<> mouseUp(CallInfo* callInfo);
	VariantReference<> mouseClick(CallInfo* callInfo);
	VariantReference<> mouseClickDrag(CallInfo* callInfo);
	VariantReference<> mouseGetPos(CallInfo* callInfo);
	VariantReference<> mouseWheel(CallInfo* callInfo);
	VariantReference<> mouseGetCursor(CallInfo* callInfo);
};

