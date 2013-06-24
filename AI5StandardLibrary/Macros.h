#pragma once
class Variant;
class StackMachine;
class Macros
{
public:
	static void registerMacros(StackMachine*);
private:
	static Variant* MyPID();
	static Variant* CRLF();
	static Variant* CR();
	static Variant* LF();
	static Variant* TAB();
	static Variant* Username();
	static Variant* WorkingDirectory();
	static Variant* TempDirectory();
	static Variant* SystemDirectory();
	static Variant* WindowsDirectory();
	static Variant* millisecond();
	static Variant* second();
	static Variant* minute();
	static Variant* hour();
	static Variant* monthDay();
	static Variant* month();
	static Variant* year();
	static Variant* weekDay();
};

