#pragma once
#include "variant.h"



class Integer64Variant: public Variant
{
public:
	Integer64Variant(const __int64 integer);
	virtual double toFloating();
	virtual __int64 toInteger();
	virtual bool toBoolean();
	virtual void print();
	virtual shared_string toString();
	friend class VariantFactory;
	__forceinline __int64 getValue();
	static Integer64Variant* createFromFactory(VariantFactory* factory,__int64 value);
private:
	__int64 value;
	
};


__int64 Integer64Variant::getValue()
{
	return value;
}


