#pragma once
#include "functions.h"

class Variant;
class StackMachine;

class StringFunctions :
	public Functions
{
public:
	static void registerFunctions(StackMachine* machine);
	~StringFunctions(void);
private:
	StringFunctions(void);
	Variant* stringlower(Variant** args,int argsSize);
	Variant* stringupper(Variant** args,int argsSize);
	Variant* stringlen(Variant** args,int argsSize);
};

