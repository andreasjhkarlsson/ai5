#pragma once
#include "variant.h"
class BooleanVariant :
	public Variant
{
public:
	virtual double toFloating();
	virtual __int64 toInteger();
	virtual bool toBoolean();
	virtual void print();
	static BooleanVariant True;
	static BooleanVariant False;
	const bool value;
private:
	BooleanVariant(bool value);
	~BooleanVariant(void);
};

