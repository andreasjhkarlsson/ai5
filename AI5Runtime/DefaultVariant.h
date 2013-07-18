#pragma once
#include "variant.h"
class DefaultVariant :
	public Variant
{
public:
	static const VARIANT_TYPE TYPE = DEFAULT;
	~DefaultVariant(void);
	virtual std::wostream& format(std::wostream& stream) const;
	virtual double toFloating() const;
	virtual __int64 toInteger64() const;
	virtual int toInteger32() const;
	virtual bool toBoolean() const;
	virtual shared_string toString() const;
	virtual bool equal(Variant*);
	friend class VariantReference<>;
private:
	DefaultVariant(void);
};

