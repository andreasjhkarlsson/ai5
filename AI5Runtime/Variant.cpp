#include "Variant.h"
#include <iostream>



Variant::Variant(const VARIANT_TYPE type): refCount(1), type(type), recycler(nullptr)
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