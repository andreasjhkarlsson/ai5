#pragma once
#include "functions.h"
class Variant;
class StackMachine;
class VariableFunctions :
	public Functions
{
public:
	~VariableFunctions(void);
	static void registerFunctions(StackMachine* machine);
private:
	VariableFunctions(void);
	Variant* stringToBinary(Variant** args,int argCount);
	Variant* binaryToString(Variant** args,int argCount);
};

