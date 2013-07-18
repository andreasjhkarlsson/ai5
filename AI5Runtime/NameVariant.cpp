#include "NameVariant.h"
#include <iostream>
#include "RuntimeError.h"
#include "VariantReference.h"

NameVariant::NameVariant(const VariantReference<>& value,VARIANT_TYPE type): Variant(type), isConst(false),value(value)
{
	this->setValue(value);
}

NameVariant::~NameVariant(void)
{
	cleanup();
}

std::wostream& NameVariant::format(std::wostream& stream) const
{
	stream << L"NameVariant ";
	if(!value.empty())
		value.format(stream);
	return stream;
}

double NameVariant::toFloating() const
{
	return 0.0;
}

__int64 NameVariant::toInteger64() const
{
	return 0;
}

int NameVariant::toInteger32() const
{
	return 0;
}

bool NameVariant::toBoolean() const
{
	return !value.empty();
}

shared_string NameVariant::toString() const
{
	return shared_string(new UnicodeString(L""));

}

void NameVariant::cleanup()
{
	Variant::cleanup();
}

const VariantReference<>& NameVariant::getValue()
{
	return value;
}

bool NameVariant::isConstName()
{
	return isConst;
}

void NameVariant::setValue(const VariantReference<>& newVariant)
{
	if(isConst)
	{
		throw RuntimeError(L"Cannot set value of const name!");
	}

	value = newVariant;	
}

void NameVariant::markAsConst()
{
	// Cannot set name as const several times.
	if(isConst)
	{
		throw RuntimeError(L"Cannot set name as const multiple times!");
	}
	isConst = true;
}

bool NameVariant::equal(Variant* other)
{
	// Only compare equal for exactly the same instance.
	return this == other;
}

