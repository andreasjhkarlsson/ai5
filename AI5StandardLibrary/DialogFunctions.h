#pragma once

class StackMachine;
class Variant;
class CallInfo;

class DialogFunctions
{
public:
	static void registerFunctions(StackMachine* machine);
	~DialogFunctions(void);
private:
	DialogFunctions(void);
	Variant* msgbox(CallInfo* callInfo);
};

