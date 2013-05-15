#pragma once

#include "FastStack.h"
#include "NameStorage.h"

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

	void attachName(Name* name)
	{
		locals.push(name);

	}
	void detachNames()
	{
		while(locals.size() > 0)
		{
			locals.pop()->popLocal();
		}
	}

	void setReturnAddress(int address)
	{
		this->returnAddress = address;
	}

private:
	int returnAddress;
	FastStack<Name*> locals;
};

