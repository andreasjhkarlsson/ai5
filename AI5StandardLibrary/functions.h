#pragma once
class Variant;

namespace AI5StandardLibrary
{

Variant* hello(Variant** args,int argsSize);
Variant* sleep(Variant** args,int argsSize);
Variant* printline(Variant** args,int argsSize);
Variant* getint(Variant** args,int argsSize);
Variant* ubound(Variant** args,int argsSize);

Variant* arrayadd(Variant** args,int argsSize);

class Macros
{
public:
	static Variant* MyPID(StackMachine*);
	static Variant* StackSize(StackMachine*);
};

}


