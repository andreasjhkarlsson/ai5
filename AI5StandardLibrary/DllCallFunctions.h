#pragma once
#include "functions.h"

class Variant;
class StackMachine;

class DllCallFunctions : public Functions
{
public:
	static void registerFunctions(StackMachine* machine);
	~DllCallFunctions();
private:
	DllCallFunctions();
	Variant* dllcall(Variant** args, int argsSize);
	Variant* dllcalladdress(Variant** args, int argsSize);
	Variant* dllopen(Variant** args, int argsSize);
	Variant* dllclose(Variant** args, int argsSize);
};

