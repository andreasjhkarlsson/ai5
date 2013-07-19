#pragma once
#include "..\AI5Runtime\HandleVariant.h"
#include <Windows.h>

class ModuleHandle: public HandleVariant
{
public:
	static const HANDLE_TYPE HTYPE = MODULE_HANDLE;
	ModuleHandle(HMODULE module);
	~ModuleHandle();
	void close();
	virtual bool isValid() const override;
	HMODULE getModule();
private:
	HMODULE module;
};



