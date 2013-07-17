#pragma once

#include "..\AI5Runtime\types.h"

class GraphicsAndSoundFunctions
{
public:
	static void registerFunctions(StackMachine* machine);
	~GraphicsAndSoundFunctions(void);
private:
	GraphicsAndSoundFunctions(void);
	VariantReference<> beep(CallInfo* callInfo);
};

