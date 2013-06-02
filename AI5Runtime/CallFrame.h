#pragma once
#include "FastStack.h"
#include "Scope.h"

class StackMachine;

class CallFrame
{
public:
	CallFrame();
	~CallFrame(void);

	int getReturnAddress()
	{
		return returnAddress;
	}

	void setReturnAddress(int address)
	{
		this->returnAddress = address;
	}

	void setScope(Scope* scope)
	{
		this->scope = scope;
	}

	Scope* getScope()
	{
		return scope;
	}

private:
	int returnAddress;
	Scope* scope;
};

