#pragma once
#include "variant.h"
class StackMachineThread;
class FloatingVariant:
	public Variant
{
public:
	static const VARIANT_TYPE TYPE = FLOATING;
	
	~FloatingVariant(void);
	virtual double toFloating() const;
	virtual __int64 toInteger64() const;
	virtual int toInteger32() const;
	virtual bool toBoolean() const;
	virtual bool equal(Variant*);
	std::wostream& format(std::wostream& stream) const;
	virtual shared_string toString() const;
	virtual size_t hash() const;
	double getValue()
	{
		return value;
	}
	void setValue(double value)
	{
		this->value = value;
	}
	friend class VariantReference<>;
private:
	FloatingVariant(double value);
	double value;
};

