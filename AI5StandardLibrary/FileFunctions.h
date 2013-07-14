#pragma once
class Variant;
class StackMachine;
class CallInfo;

class FileFunctions
{
public:
	~FileFunctions(void);
	static void registerFunctions(StackMachine* machine);
private:
	FileFunctions(void);
	Variant* printline(CallInfo* callInfo);
	Variant* fileExists(CallInfo* callInfo);
	Variant* fileChangeDir(CallInfo* callInfo);
	Variant* consoleWrite(CallInfo* callInfo);
	Variant* consoleWriteError(CallInfo* callInfo);
	Variant* consoleReadLine(CallInfo* callInfo);
	Variant* fileOpen(CallInfo* callInfo);
	Variant* fileClose(CallInfo* callInfo);
	Variant* fileRead(CallInfo* callInfo);
	Variant* fileWrite(CallInfo* callInfo);
	Variant* fileReadLine(CallInfo* callInfo);
};

