#pragma once
#include "namevariant.h"
#include "PooledObject.h"
class NameReferenceVariant :
	public NameVariant
{
public:
	static const VARIANT_TYPE TYPE = NAME_REFERENCE;
	NameReferenceVariant(Variant* value);
	~NameReferenceVariant(void);
	virtual void setValue(Variant* var);
	virtual Variant* getValue();
	static const int POOL_SIZE = 1024;
};

