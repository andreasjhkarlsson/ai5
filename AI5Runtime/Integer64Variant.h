#pragma once
#include "variant.h"



class Integer64Variant: public Variant
{
public:
	static const VARIANT_TYPE TYPE = INTEGER64;
	
	virtual double toFloating() const;
	virtual __int64 toInteger64() const;
	virtual int toInteger32() const;
	virtual bool toBoolean() const;
	virtual bool equal(Variant*);
	std::wostream& format(std::wostream& stream) const;
	virtual shared_string toString() const;
	virtual size_t hash() const;
	friend class VariantReference<>;
	__int64 getValue();
	void setValue(__int64 value);
private:
	__int64 value;
	Integer64Variant(const __int64 integer);
};





