#include <iostream>
#include "Variant.h"
#include "NameVariant.h"

Variant::Variant(const VARIANT_TYPE type): refCount(1), type(type), recycler(nullptr), lastName(nullptr)
{
}

Variant::~Variant(void)
{
}

VariantFactory::VariantFactory(void)
{
	for(int i=0;i<Variant::NUMBER_OF_VARIANT_TYPES;i++)
	{
		recycleBins.push_back(new FastStack<Variant*>(VariantFactory::RECYCLE_BIN_LIMIT));
	}
}
VariantFactory::~VariantFactory(void)
{
}

void Variant::cleanup()
{
}

void Variant::setLastName(NameVariant* name)
{
	// Do NOT increase ref count as that would create a circular reference.
	// Instead, rely on the fact that this reference by names which are reset.
	this->lastName = name;
}

NameVariant* Variant::getLastName()
{
	return lastName;
}