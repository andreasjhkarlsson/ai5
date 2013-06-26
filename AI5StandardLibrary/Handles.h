#pragma once
#include "..\AI5Runtime\HandleVariant.h"
#include <Windows.h>

class ModuleHandle: public HandleVariant
{
public:
	static const HANDLE_TYPE HTYPE = MODULE_HANDLE;
	ModuleHandle(HMODULE module);
	void cleanup();
	void close();
	HMODULE getModule();
private:
	HMODULE module;
};



