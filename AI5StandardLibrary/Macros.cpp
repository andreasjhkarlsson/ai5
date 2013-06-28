#include "Macros.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\Integer64Variant.h"
#include "..\AI5Runtime\StringVariant.h"
#include <Windows.h>
#include <vector>
#include <Lmcons.h>


void Macros::registerMacros(StackMachine* machine)
{
	machine->addMacro(L"autoitpid",&MyPID);
	machine->addMacro(L"crlf",&CRLF);
	machine->addMacro(L"lf",&LF);
	machine->addMacro(L"cr",&CR);
	machine->addMacro(L"tab",&TAB);
	machine->addMacro(L"username",&Username);
	machine->addMacro(L"workingdir",&WorkingDirectory);
	machine->addMacro(L"tempdir",&TempDirectory);
	machine->addMacro(L"systemdir",&SystemDirectory);
	machine->addMacro(L"windowsdir",&WindowsDirectory);
	machine->addMacro(L"msec",&millisecond);
	machine->addMacro(L"sec",&second);
	machine->addMacro(L"min",&minute);
	machine->addMacro(L"hour",&hour);
	machine->addMacro(L"mday",&monthDay);
	machine->addMacro(L"mon",&month);
	machine->addMacro(L"year",&year);
	machine->addMacro(L"wday",&weekDay);
	machine->addMacro(L"error",&errorCode);
	machine->addMacro(L"extended",&extendedCode);
}

Variant* Macros::MyPID(StackMachine*)
{
	return new Integer64Variant(GetCurrentProcessId());
}

Variant* Macros::errorCode(StackMachine* machine)
{
	Variant* vError = machine->getErrorCode();
	vError->addRef();
	return vError;
}

Variant* Macros::extendedCode(StackMachine* machine)
{
	Variant* vExtended = machine->getExtendedCode();
	vExtended->addRef();
	return vExtended;
}

Variant* Macros::CRLF(StackMachine*)
{
	static StringVariant crlf(L"\r\n");
	crlf.addRef();
	return &crlf;
}
Variant* Macros::CR(StackMachine*)
{
	static StringVariant cr(L"\r");
	cr.addRef();
	return &cr;
}
Variant* Macros::LF(StackMachine*)
{
	static StringVariant lf(L"\n");
	lf.addRef();
	return &lf;
}
Variant* Macros::TAB(StackMachine*)
{
	static StringVariant tab(L"\t");
	tab.addRef();
	return &tab;
}


Variant* Macros::Username(StackMachine*)
{
	DWORD buffSize = UNLEN+1;
	std::vector<wchar_t> buffer(buffSize);
	GetUserNameW(&buffer[0],&buffSize);
	return new StringVariant(&buffer[0]);

}

Variant* Macros::WorkingDirectory(StackMachine*)
{
	std::vector<wchar_t> buffer(MAX_PATH);
	GetCurrentDirectoryW(MAX_PATH,&buffer[0]);
	return new StringVariant(&buffer[0]);
}

Variant* Macros::TempDirectory(StackMachine*)
{
	std::vector<wchar_t> buffer(MAX_PATH);
	GetTempPathW(MAX_PATH,&buffer[0]);
	return new StringVariant(&buffer[0]);
}


Variant* Macros::SystemDirectory(StackMachine*)
{
	std::vector<wchar_t> buffer(MAX_PATH);
	GetSystemDirectoryW(&buffer[0],MAX_PATH);
	return new StringVariant(&buffer[0]);
}
Variant* Macros::WindowsDirectory(StackMachine*)
{
	std::vector<wchar_t> buffer(MAX_PATH);
	GetWindowsDirectoryW(&buffer[0],MAX_PATH);
	return new StringVariant(&buffer[0]);
}


Variant* Macros::millisecond(StackMachine*)
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return new Integer32Variant(stime.wMilliseconds);
}

Variant* Macros::second(StackMachine*)
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return new Integer32Variant(stime.wSecond);
}
Variant* Macros::minute(StackMachine*)
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return new Integer32Variant(stime.wMinute);
}
Variant* Macros::hour(StackMachine*)
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return new Integer32Variant(stime.wHour);
}
Variant* Macros::monthDay(StackMachine*)
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return new Integer32Variant(stime.wDay);
}
Variant* Macros::month(StackMachine*)
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return new Integer32Variant(stime.wMonth);
}
Variant* Macros::year(StackMachine*)
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return new Integer32Variant(stime.wYear);
}
Variant* Macros::weekDay(StackMachine*)
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return new Integer32Variant(stime.wDayOfWeek);
}
