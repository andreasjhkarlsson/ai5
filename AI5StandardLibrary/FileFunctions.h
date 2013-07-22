#pragma once
#include "..\AI5Runtime\types.h"
#include <mutex>

class FileFunctions
{
public:
	~FileFunctions(void);
	static void registerFunctions(StackMachine* machine);
private:
	FileFunctions(void);
	VariantReference<> printline(CallInfo* callInfo);
	VariantReference<> fileExists(CallInfo* callInfo);
	VariantReference<> fileChangeDir(CallInfo* callInfo);
	VariantReference<> consoleWrite(CallInfo* callInfo);
	VariantReference<> consoleWriteError(CallInfo* callInfo);
	VariantReference<> consoleReadLine(CallInfo* callInfo);
	VariantReference<> fileOpen(CallInfo* callInfo);
	VariantReference<> fileClose(CallInfo* callInfo);
	VariantReference<> fileRead(CallInfo* callInfo);
	VariantReference<> fileWrite(CallInfo* callInfo);
	VariantReference<> fileReadLine(CallInfo* callInfo);
	std::recursive_mutex printLock;
};

