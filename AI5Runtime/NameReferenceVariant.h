#pragma once
#include "namevariant.h"
#include "PooledObject.h"
class NameReferenceVariant :
	public NameVariant
{
public:
	static const VARIANT_TYPE TYPE = NAME_REFERENCE;
	NameReferenceVariant(const VariantReference<>& value);
	~NameReferenceVariant(void);
	virtual void setValue(const VariantReference<>& var);
	virtual const VariantReference<>& getValue();
	static const int POOL_SIZE = 1024;
};

