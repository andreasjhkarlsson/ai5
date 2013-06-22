#pragma once
#include "functions.h"

class StackMachine;
class Variant;
class DialogFunctions :
	public Functions
{
public:
	static void registerFunctions(StackMachine* machine);
	~DialogFunctions(void);
private:
	DialogFunctions(void);
	Variant* msgbox(Variant** args,int argCount);
};

