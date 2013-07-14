#pragma once

class StackMachineThread;
class Variant;
class CallInfo;

class DialogFunctions
{
public:
	static void registerFunctions(StackMachineThread* machine);
	~DialogFunctions(void);
private:
	DialogFunctions(void);
	Variant* msgbox(CallInfo* callInfo);
};

