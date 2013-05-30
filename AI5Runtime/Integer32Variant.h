#pragma once
#include "variant.h"
class Integer32Variant :
	public Variant
{
public:
	Integer32Variant(int value);
	~Integer32Variant(void);
	virtual void print();
	virtual double toFloating();
	virtual __int64 toInteger64();
	virtual int toInteger32();
	virtual bool toBoolean();
	virtual shared_string toString();
private:
	int value;
};

