#pragma once
#include "variant.h"



class IntegerVariant: public Variant
{
public:
	IntegerVariant(const __int64 integer);
	virtual double toFloating();
	virtual __int64 toInteger();
	virtual bool toBoolean();
	virtual void print();
	virtual shared_string toString();
	static IntegerVariant** BYTE_TABLE;
	static bool INITIALIZED;
	static bool INITIALIZE();
	friend class VariantFactory;
	__forceinline __int64 getValue();
	static IntegerVariant* createFromFactory(VariantFactory* factory,__int64 value);
private:
	__int64 value;
	
};


__int64 IntegerVariant::getValue()
{
	return value;
}


