#pragma once
#include "variant.h"
class IteratorVariant :
	public Variant
{
public:
	~IteratorVariant(void);
	virtual bool hasMore()=0;
	virtual Variant* next()=0;
protected:
	IteratorVariant(void);
};

