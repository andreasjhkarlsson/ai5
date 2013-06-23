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
	Variant* stringLower(Variant** args,int argsSize);
	Variant* stringUpper(Variant** args,int argsSize);
	Variant* stringLen(Variant** args,int argsSize);
	Variant* stringLeft(Variant** args,int argsSize);
	Variant* stringTrimLeft(Variant** args,int argsSize);
	Variant* stringRight(Variant** args,int argsSize);
	Variant* stringTrimRight(Variant** args,int argsSize);
	Variant* stringIsDigit(Variant** args,int argsSize);
};

