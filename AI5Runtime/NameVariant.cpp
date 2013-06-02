#include "NameVariant.h"
#include <iostream>

NameVariant::NameVariant(Variant* value): Variant(NAME), isConst(false)
{
	this->setValue(value);
}

NameVariant::~NameVariant(void)
{
}

void NameVariant::print()
{
	std::wcout << L"NameVariant" << std::endl;
}
double NameVariant::toFloating()
{
	return 0.0;
}
__int64 NameVariant::toInteger64()
{
	return 0;
}
int NameVariant::toInteger32()
{
	return 0;
}
bool NameVariant::toBoolean()
{
	return value != nullptr;
}
shared_string NameVariant::toString()
{
	return shared_string(new std::wstring(L""));

}
void NameVariant::cleanup()
{
	if(value != nullptr)
	{
		value->release();
		value = nullptr;
		isConst = false;
	}
}

Variant* NameVariant::getValue()
{
	return value;
}
void NameVariant::setValue(Variant* newVariant)
{
	if(newVariant == nullptr)
	{
		value = nullptr;
		return;
	}
	if(isConst)
	{
		// Throw error!!!
		return;
	}
	newVariant->addRef();
	if(value != nullptr)
		value->release();
	value = newVariant;	
}

void NameVariant::markAsConst()
{
	// Cannot set name as const several times.
	if(isConst)
	{
		// Throw error!
	}
	isConst = true;
}

NameVariant* NameVariant::createFromFactory(VariantFactory* factory)
{
	return factory->create<NameVariant,Variant*>(Variant::NAME,nullptr);
}