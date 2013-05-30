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
	virtual __int64 toInteger64();
	virtual int toInteger32();
	virtual bool toBoolean();
	virtual shared_string toString();
private:
	shared_string str;
};

