#pragma once
#include "FastStack.h"
#include "Scope.h"
#include "Block.h"


class CallFrame: Block
{
public:
	CallFrame();
	~CallFrame(void);
	void setup(StackMachine* machine,int numberOfArguments);

	Scope* getScope();
	virtual void leave(StackMachine*);

private:
	int returnAddress;
	Scope scope;
	int numberOfArguments;
};

