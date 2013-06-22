#pragma once
#include "functions.h"
class Variant;
class StackMachine;
class FileFunctions :
	public Functions
{
public:
	~FileFunctions(void);
	static void registerFunctions(StackMachine* machine);
private:
	FileFunctions(void);
	Variant* printline(Variant** args,int argsSize);
};

