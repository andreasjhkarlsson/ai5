#pragma once

class Variant;
class StackMachine;
class CallInfo;

class DllCallFunctions
{
public:
	static void registerFunctions(StackMachine* machine);
	~DllCallFunctions();
private:
	DllCallFunctions();
	Variant* dllcall(CallInfo* callInfo);
	Variant* dllcalladdress(CallInfo* callInfo);
	Variant* dllopen(CallInfo* callInfo);
	Variant* dllclose(CallInfo* callInfo);
};

