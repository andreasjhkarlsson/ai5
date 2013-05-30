#pragma once
#include "variant.h"
class StackMachine;
class FunctionVariant :
	public Variant
{
public:
	FunctionVariant(VARIANT_TYPE type);
	~FunctionVariant(void);
	virtual double toFloating()
	{
		return 0.0;
	}
	virtual __int64 toInteger64()
	{
		return 0;
	}
	virtual int toInteger32()
	{
		return 0;
	}
	virtual bool toBoolean()
	{
		return true;
	}

	virtual shared_string toString()
	{
		return create_shared_string(L"");
	}
};

