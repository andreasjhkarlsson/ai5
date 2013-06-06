#pragma once
#include "FastStack.h"
#include "Scope.h"
#include "Block.h"


class CallFrame: Block
{
public:
	CallFrame(StackMachine* machine,int numberOfArguments);
	~CallFrame(void);

	Scope* getScope();
	virtual void leave(StackMachine*);

private:
	int returnAddress;
	Scope scope;
	int numberOfArguments;
};

