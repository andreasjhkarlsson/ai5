#pragma once
#include "functions.h"
class Variant;
class StackMachine;
class ListFunctions :
	public Functions
{
public:
	static void registerFunctions(StackMachine* machine);
	~ListFunctions(void);
private:
	ListFunctions(void);
	Variant* ubound(Variant** args,int argsSize);
	Variant* arrayadd(Variant** args,int argsSize);
};

