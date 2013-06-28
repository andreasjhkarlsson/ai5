#pragma once
class Variant;
class StackMachine;
class Macros
{
public:
	static void registerMacros(StackMachine*);
private:
	static Variant* MyPID(StackMachine*);
	static Variant* CRLF(StackMachine*);
	static Variant* CR(StackMachine*);
	static Variant* LF(StackMachine*);
	static Variant* TAB(StackMachine*);
	static Variant* Username(StackMachine*);
	static Variant* WorkingDirectory(StackMachine*);
	static Variant* TempDirectory(StackMachine*);
	static Variant* SystemDirectory(StackMachine*);
	static Variant* WindowsDirectory(StackMachine*);
	static Variant* millisecond(StackMachine*);
	static Variant* second(StackMachine*);
	static Variant* minute(StackMachine*);
	static Variant* hour(StackMachine*);
	static Variant* monthDay(StackMachine*);
	static Variant* month(StackMachine*);
	static Variant* year(StackMachine*);
	static Variant* weekDay(StackMachine*);
	static Variant* errorCode(StackMachine*);
	static Variant* extendedCode(StackMachine*);
};

