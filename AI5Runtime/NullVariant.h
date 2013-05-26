#pragma once
#include "variant.h"
class NullVariant :
	public Variant
{
public:
	static NullVariant Instance;
	~NullVariant(void);
	virtual void print();
	virtual double toFloating();
	virtual __int64 toInteger();
	virtual bool toBoolean();
	virtual shared_string toString();
private:
	NullVariant(void);
};

