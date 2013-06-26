#include <iostream>
#include "Variant.h"
#include "NameVariant.h"
#include "IteratorVariant.h"

Variant::Variant(const VARIANT_TYPE type,bool isContainer): refCount(1), type(type), recycler(nullptr), isContainer(isContainer)
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
//	std::wcout << "Cleaning up ";
//	this->format(std::wcout);
//	std::wcout << std::endl;
}


std::wostream& Variant::format(std::wostream& stream) const
{
	stream << L"No formatting available";
	return stream;
}
double Variant::toFloating() const
{
	return 0.0;
}
__int64 Variant::toInteger64() const
{
	return 0;
}
int Variant::toInteger32() const
{
	return 0;
}
bool Variant::toBoolean() const
{
	return false;
}
shared_string Variant::toString() const
{
	return shared_string(new std::wstring(L""));
}
bool Variant::equal(Variant*)
{
	return false;
}


size_t Variant::hash() const
{
	return (size_t)this;
}


IteratorVariant* Variant::iterate()
{
	throw RuntimeError(L"Type not iterable");
}