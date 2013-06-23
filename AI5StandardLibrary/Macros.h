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
};

