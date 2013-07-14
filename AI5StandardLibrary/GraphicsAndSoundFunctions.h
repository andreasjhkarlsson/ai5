#pragma once

class StackMachineThread;
class Variant;
class CallInfo;
class GraphicsAndSoundFunctions
{
public:
	static void registerFunctions(StackMachineThread* machine);
	~GraphicsAndSoundFunctions(void);
private:
	GraphicsAndSoundFunctions(void);
	Variant* beep(CallInfo* callInfo);
};

