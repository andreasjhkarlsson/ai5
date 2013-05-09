#pragma once

#include <vector>

class StackMachine;

class CallFrame
{
public:
	CallFrame(int returnAddress);
	~CallFrame(void);

	int getReturnAddress()
	{
		return returnAddress;
	}

	void rememberName(int index);
	void popLocals(StackMachine* machine);

private:
	int returnAddress;
	std::vector<int> locals;
};

