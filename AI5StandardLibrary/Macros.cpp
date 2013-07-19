#include "Macros.h"
#include "..\AI5Runtime\StackMachineThread.h"
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

VariantReference<> Macros::MyPID(StackMachineThread*)
{
	return (int)GetCurrentProcessId();
}

VariantReference<> Macros::errorCode(StackMachineThread* machine)
{
	VariantReference<> vError = machine->getErrorCode();
	return vError;
}

VariantReference<> Macros::extendedCode(StackMachineThread* machine)
{
	VariantReference<> vExtended = machine->getExtendedCode();
	return vExtended;
}

VariantReference<> Macros::CRLF(StackMachineThread*)
{
	static StringVariant* crlf = StringVariant::CreateStatic(L"\r\n");
	return crlf;
}
VariantReference<> Macros::CR(StackMachineThread*)
{
	static StringVariant* cr = StringVariant::CreateStatic(L"\r");
	return cr;
}
VariantReference<> Macros::LF(StackMachineThread*)
{
	static StringVariant* lf = StringVariant::CreateStatic(L"\n");
	return lf;
}
VariantReference<> Macros::TAB(StackMachineThread*)
{
	static StringVariant* tab = StringVariant::CreateStatic(L"\t");
	return tab;
}


VariantReference<> Macros::Username(StackMachineThread*)
{
	DWORD buffSize = UNLEN+1;
	std::vector<wchar_t> buffer(buffSize);
	GetUserNameW(&buffer[0],&buffSize);
	return StringVariant::Create(&buffer[0]);

}

VariantReference<> Macros::WorkingDirectory(StackMachineThread*)
{
	std::vector<wchar_t> buffer(MAX_PATH);
	GetCurrentDirectoryW(MAX_PATH,&buffer[0]);
	return StringVariant::Create(&buffer[0]);
}

VariantReference<> Macros::TempDirectory(StackMachineThread*)
{
	std::vector<wchar_t> buffer(MAX_PATH);
	GetTempPathW(MAX_PATH,&buffer[0]);
	return StringVariant::Create(&buffer[0]);
}


VariantReference<> Macros::SystemDirectory(StackMachineThread*)
{
	std::vector<wchar_t> buffer(MAX_PATH);
	GetSystemDirectoryW(&buffer[0],MAX_PATH);
	return StringVariant::Create(&buffer[0]);
}
VariantReference<> Macros::WindowsDirectory(StackMachineThread*)
{
	std::vector<wchar_t> buffer(MAX_PATH);
	GetWindowsDirectoryW(&buffer[0],MAX_PATH);
	return StringVariant::Create(&buffer[0]);
}


VariantReference<> Macros::millisecond(StackMachineThread*)
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return stime.wMilliseconds;
}

VariantReference<> Macros::second(StackMachineThread*)
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return stime.wSecond;
}
VariantReference<> Macros::minute(StackMachineThread*)
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return stime.wMinute;
}
VariantReference<> Macros::hour(StackMachineThread*)
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return stime.wHour;
}
VariantReference<> Macros::monthDay(StackMachineThread*)
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return stime.wDay;
}
VariantReference<> Macros::month(StackMachineThread*)
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return stime.wMonth;
}
VariantReference<> Macros::year(StackMachineThread*)
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return stime.wYear;
}
VariantReference<> Macros::weekDay(StackMachineThread*)
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	return stime.wDayOfWeek;
}
