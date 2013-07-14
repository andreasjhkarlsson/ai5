#pragma once

class StackMachine;
class Variant;
class CallInfo;
class GraphicsAndSoundFunctions
{
public:
	static void registerFunctions(StackMachine* machine);
	~GraphicsAndSoundFunctions(void);
private:
	GraphicsAndSoundFunctions(void);
	Variant* beep(CallInfo* callInfo);
};

