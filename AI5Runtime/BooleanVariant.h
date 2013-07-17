#pragma once
#include "variant.h"
class BooleanVariant :
	public Variant
{
public:
	static const VARIANT_TYPE TYPE = BOOLEAN;
	virtual double toFloating() const;
	virtual __int64 toInteger64() const;
	virtual int toInteger32() const;
	virtual bool toBoolean() const;
	virtual bool equal(Variant*);
	std::wostream& format(std::wostream& stream) const;
	static BooleanVariant True;
	static BooleanVariant False;
	const bool value;
	virtual shared_string toString() const;
	friend class VariantReference<>;
private:
	static BooleanVariant* Get(bool value,bool increaseRefcount=false);
	BooleanVariant(bool value);
	~BooleanVariant(void);
};

