#pragma once
#include "..\AI5Runtime\HandleVariant.h"
#include <Windows.h>

class ModuleHandle: public HandleVariant
{
public:
	friend class GC;
	static const HANDLE_TYPE HTYPE = MODULE_HANDLE;
	~ModuleHandle();
	void close();
	virtual bool isValid() const override;
	HMODULE getModule();
private:
	ModuleHandle(HMODULE module);
	HMODULE module;
};



