#pragma once
#include "functions.h"
class StackMachine;
class Variant;
class GraphicsAndSoundFunctions :
	public Functions
{
public:
	static void registerFunctions(StackMachine* machine);
	~GraphicsAndSoundFunctions(void);
private:
	GraphicsAndSoundFunctions(void);
	Variant* beep(Variant** args,int argsSize);
};

