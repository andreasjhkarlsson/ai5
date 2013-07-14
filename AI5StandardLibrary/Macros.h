#pragma once
class Variant;
class StackMachine;
class StackMachineThread;
class Macros
{
public:
	static void registerMacros(StackMachine*);
private:
	static Variant* MyPID(StackMachineThread*);
	static Variant* CRLF(StackMachineThread*);
	static Variant* CR(StackMachineThread*);
	static Variant* LF(StackMachineThread*);
	static Variant* TAB(StackMachineThread*);
	static Variant* Username(StackMachineThread*);
	static Variant* WorkingDirectory(StackMachineThread*);
	static Variant* TempDirectory(StackMachineThread*);
	static Variant* SystemDirectory(StackMachineThread*);
	static Variant* WindowsDirectory(StackMachineThread*);
	static Variant* millisecond(StackMachineThread*);
	static Variant* second(StackMachineThread*);
	static Variant* minute(StackMachineThread*);
	static Variant* hour(StackMachineThread*);
	static Variant* monthDay(StackMachineThread*);
	static Variant* month(StackMachineThread*);
	static Variant* year(StackMachineThread*);
	static Variant* weekDay(StackMachineThread*);
	static Variant* errorCode(StackMachineThread*);
	static Variant* extendedCode(StackMachineThread*);
};

