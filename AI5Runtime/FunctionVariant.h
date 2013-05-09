#pragma once
#include "variant.h"
class StackMachine;
class FunctionVariant :
	public Variant
{
public:
	FunctionVariant(void);
	~FunctionVariant(void);
	virtual double toFloating()
	{
		return 0.0;
	}
	virtual __int64 toInteger()
	{
		return 0;
	}
	virtual bool toBoolean()
	{
		return true;
	}
	virtual void call(StackMachine* machine)=0;
};

