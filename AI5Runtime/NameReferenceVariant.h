#pragma once
#include "namevariant.h"
#include "PooledObject.h"
class NameReferenceVariant :
	public NameVariant
{
public:
	friend class GC;
	static const VARIANT_TYPE TYPE = NAME_REFERENCE;
	
	~NameReferenceVariant(void);
	virtual void setValue(const VariantReference<>& var);
	virtual const VariantReference<>& getValue();
	static const int POOL_SIZE = 1024;
private:
	NameReferenceVariant(const VariantReference<>& value);
};

