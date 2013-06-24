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


Variant* Macros::Username()
{
	DWORD buffSize = UNLEN+1;
	std::vector<wchar_t> buffer(buffSize);
	GetUserNameW(&buffer[0],&buffSize);
	return new StringVariant(&buffer[0]);

}

Variant* Macros::WorkingDirectory()
{
	std::vector<wchar_t> buffer(MAX_PATH);
	GetCurrentDirectoryW(MAX_PATH,&buffer[0]);
	return new StringVariant(&buffer[0]);
}

Variant* Macros::TempDirectory()
{
	std::vector<wchar_t> buffer(MAX_PATH);
	GetTempPathW(MAX_PATH,&buffer[0]);
	return new StringVariant(&buffer[0]);
}


Variant* Macros::SystemDirectory()
{
	std::vector<wchar_t> buffer(MAX_PATH);
	GetSystemDirectoryW(&buffer[0],MAX_PATH);
	return new StringVariant(&buffer[0]);
}
Variant* Macros::WindowsDirectory()
{
	std::vector<wchar_t> buffer(MAX_PATH);
	GetWindowsDirectoryW(&buffer[0],MAX_PATH);
	return new StringVariant(&buffer[0]);
}


Variant* Macros::millisecond()
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return new Integer32Variant(stime.wMilliseconds);
}

Variant* Macros::second()
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return new Integer32Variant(stime.wSecond);
}
Variant* Macros::minute()
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return new Integer32Variant(stime.wMinute);
}
Variant* Macros::hour()
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return new Integer32Variant(stime.wHour);
}
Variant* Macros::monthDay()
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return new Integer32Variant(stime.wDay);
}
Variant* Macros::month()
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return new Integer32Variant(stime.wMonth);
}
Variant* Macros::year()
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return new Integer32Variant(stime.wYear);
}
Variant* Macros::weekDay()
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return new Integer32Variant(stime.wDayOfWeek);
}
