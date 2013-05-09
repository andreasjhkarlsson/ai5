#pragma once
#include "variant.h"
class FloatingVariant:
	public Variant
{
public:
	FloatingVariant(double value);
	~FloatingVariant(void);
	virtual double toFloating();
	virtual __int64 toInteger();
	virtual bool toBoolean();
	virtual void print();
private:
	double value;
};

