#pragma once
#include "variant.h"
class StackMachineThread;
class FunctionVariant :
	public Variant
{
public:
	FunctionVariant(VARIANT_TYPE type,bool verbosePrint=true);
	~FunctionVariant(void);
	virtual double toFloating() const
	{
		return 0.0;
	}
	virtual __int64 toInteger64() const
	{
		return 0;
	}
	virtual int toInteger32() const
	{
		return 0;
	}
	virtual bool toBoolean() const
	{
		return true;
	}

	virtual shared_string toString() const
	{
		return create_shared_string(L"");
	}
};

