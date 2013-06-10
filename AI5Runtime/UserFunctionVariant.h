#pragma once
#include "functionvariant.h"
class UserFunctionVariant :
	public FunctionVariant
{
public:
	UserFunctionVariant(int address);
	~UserFunctionVariant(void);
	std::wostream& format(std::wostream& stream);
	int getAddress();
	virtual bool equal(Variant*);
private:
	int address;
};
			
