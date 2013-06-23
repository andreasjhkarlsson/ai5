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
	Variant* fileExists(Variant** args,int argsSize);
	Variant* fileChangeDir(Variant** args,int argsSize);
	Variant* consoleWrite(Variant** args,int argSize);
	Variant* consoleWriteError(Variant** args,int argSize);
	Variant* consoleReadLine(Variant** args,int argSize);
};

