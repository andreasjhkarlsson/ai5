#pragma once
#include "functionvariant.h"
class UserFunctionVariant :
	public FunctionVariant
{
public:
	UserFunctionVariant(int address);
	~UserFunctionVariant(void);
	void print();
	int getAddress();
private:
	int address;
};
			
