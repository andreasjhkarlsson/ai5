#include "Handles.h"


ModuleHandle::ModuleHandle(HMODULE module): HandleVariant(HTYPE), module(module)
{

}

ModuleHandle::~ModuleHandle()
{
	close();
}

void ModuleHandle::close()
{
	if(module != nullptr)
		FreeLibrary(module);
	module = nullptr;
}

HMODULE ModuleHandle::getModule()
{
	return module;
}

bool ModuleHandle::isValid() const
{
	return module != nullptr;
}