#include "Macros.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\Integer64Variant.h"
#include "..\AI5Runtime\StringVariant.h"
#include <Windows.h>


void Macros::registerMacros(StackMachine* machine)
{
	machine->addMacro(L"autoitpid",&MyPID);
	machine->addMacro(L"crlf",&CRLF);
	machine->addMacro(L"lf",&LF);
	machine->addMacro(L"cr",&CR);
	machine->addMacro(L"tab",&TAB);
}

Variant* Macros::MyPID()
{
	return new Integer64Variant(GetCurrentProcessId());
}


Variant* Macros::CRLF()
{
	static StringVariant crlf(L"\r\n");
	crlf.addRef();
	return &crlf;
}
Variant* Macros::CR()
{
	static StringVariant cr(L"\r");
	cr.addRef();
	return &cr;
}
Variant* Macros::LF()
{
	static StringVariant lf(L"\n");
	lf.addRef();
	return &lf;
}
Variant* Macros::TAB()
{
	static StringVariant tab(L"\t");
	tab.addRef();
	return &tab;
}