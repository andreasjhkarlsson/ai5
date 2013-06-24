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
	Variant* isFloat(Variant** args,int argCount);
	Variant* isString(Variant** args,int argCount);
	Variant* isArray(Variant** args,int argCount);
	Variant* isHashMap(Variant** args,int argCount);
	Variant* isBool(Variant** args,int argCount);
	Variant* ToString(Variant** args,int argCount);
};

