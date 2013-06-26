#include "NameReferenceVariant.h"


NameReferenceVariant::NameReferenceVariant(Variant* value): NameVariant(value,TYPE)
{
}


NameReferenceVariant::~NameReferenceVariant(void)
{
}


void NameReferenceVariant::setValue(Variant* var)
{

	if(value == nullptr)
	{
		NameVariant::setValue(var);
		return;
	}

	static_cast<NameVariant*>(value)->setValue(var);
	
}
Variant* NameReferenceVariant::getValue()
{
	return static_cast<NameVariant*>(value)->getValue();
}