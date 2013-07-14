#pragma once

class Variant;
class StackMachineThread;
class CallInfo;

class DllCallFunctions
{
public:
	static void registerFunctions(StackMachineThread* machine);
	~DllCallFunctions();
private:
	DllCallFunctions();
	Variant* dllcall(CallInfo* callInfo);
	Variant* dllcalladdress(CallInfo* callInfo);
	Variant* dllopen(CallInfo* callInfo);
	Variant* dllclose(CallInfo* callInfo);
};

