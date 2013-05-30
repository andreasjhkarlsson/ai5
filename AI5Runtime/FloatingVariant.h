#pragma once
#include "variant.h"
class StackMachine;
class FloatingVariant:
	public Variant
{
public:
	FloatingVariant(double value);
	~FloatingVariant(void);
	virtual double toFloating();
	virtual __int64 toInteger64();
	virtual int toInteger32();
	virtual bool toBoolean();
	virtual void print();
	virtual shared_string toString();
	double getValue()
	{
		return value;
	}
	friend class VariantFactory;
	static FloatingVariant* createFromFactory(VariantFactory* factory,double value);

private:
	double value;
};

