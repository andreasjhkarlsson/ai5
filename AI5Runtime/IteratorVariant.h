#pragma once
#include "variant.h"
class IteratorVariant :
	public Variant
{
public:
	static const VARIANT_TYPE TYPE = ITERATOR;
	~IteratorVariant(void);
	virtual bool hasMore()=0;
	virtual VariantReference<> next()=0;
protected:
	IteratorVariant(void);
};

