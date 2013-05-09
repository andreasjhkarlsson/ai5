#pragma once
#include "functionvariant.h"
class UserFunctionVariant :
	public FunctionVariant
{
public:
	UserFunctionVariant(int address);
	~UserFunctionVariant(void);
	void call(StackMachine* machine);
	void print();
private:
	int address;
};

