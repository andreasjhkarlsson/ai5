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
	virtual bool equal(Variant*);
	std::wostream& format(std::wostream& stream);
	virtual shared_string toString();
	virtual size_t hash();
	double getValue()
	{
		return value;
	}
	void setValue(double value)
	{
		this->value = value;
	}
	static FloatingVariant* createFromFactory(VariantFactory* factory,double value);

private:
	double value;
};

