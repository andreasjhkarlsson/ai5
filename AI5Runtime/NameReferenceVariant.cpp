#include "NameReferenceVariant.h"


NameReferenceVariant::NameReferenceVariant(const VariantReference<>& value): NameVariant(value,TYPE)
{
}


NameReferenceVariant::~NameReferenceVariant(void)
{
}

NameReferenceVariant* NameReferenceVariant::Create(const VariantReference<>& value)
{
	return GC::alloc<NameReferenceVariant,const VariantReference<>&>(value);
}

void NameReferenceVariant::setValue(const VariantReference<>& var)
{

	if(value.empty())
	{
		NameVariant::setValue(var);
		return;
	}

	value.cast<NameVariant>()->setValue(var);
	
}
const VariantReference<>& NameReferenceVariant::getValue()
{
	return value.cast<NameVariant>()->getValue();
}