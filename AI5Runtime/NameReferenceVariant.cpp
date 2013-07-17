#include "NameReferenceVariant.h"


NameReferenceVariant::NameReferenceVariant(const VariantReference<>& value): NameVariant(value,TYPE)
{
}


NameReferenceVariant::~NameReferenceVariant(void)
{
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