#pragma once
class StackMachine;
class Variant;

namespace AI5StandardLibrary
{

void hello(StackMachine*);
void sleep(StackMachine*);
void printline(StackMachine*);
void getint(StackMachine*);
void stringlen(StackMachine*);
void ubound(StackMachine*);

class Macros
{
public:
	static Variant* MyPID(StackMachine*);
	static Variant* StackSize(StackMachine*);
};

}


