#pragma once
#include "variant.h"
#include <string>
class StringVariant :
	public Variant
{
public:
	StringVariant(shared_string str);
	~StringVariant(void);
	virtual void print();
	virtual double toFloating();
	virtual __int64 toInteger();
	virtual bool toBoolean();
	virtual shared_string toString();
private:
	shared_string str;
};

