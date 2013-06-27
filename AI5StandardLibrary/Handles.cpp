#include "Handles.h"


ModuleHandle::ModuleHandle(HMODULE module): HandleVariant(HTYPE), module(module)
{

}

void ModuleHandle::cleanup()
{
	if(module)
		close();
}

void ModuleHandle::close()
{
	FreeLibrary(module);
	module = nullptr;
}

HMODULE ModuleHandle::getModule()
{
	return module;
}

bool ModuleHandle::isValid()
{
	return module != nullptr;
}