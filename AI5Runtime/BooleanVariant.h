#pragma once
#include "variant.h"
class BooleanVariant :
	public Variant
{
public:
	virtual double toFloating();
	virtual __int64 toInteger64();
	virtual int toInteger32();
	virtual bool toBoolean();
	virtual void print();
	static BooleanVariant True;
	static BooleanVariant False;
	const bool value;
	virtual shared_string toString();
private:
	BooleanVariant(bool value);
	~BooleanVariant(void);
};

