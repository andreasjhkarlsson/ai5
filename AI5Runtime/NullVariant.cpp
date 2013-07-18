#include "NullVariant.h"
#include <iostream>

NullVariant::NullVariant(void): Variant(TYPE)
{
}


NullVariant::~NullVariant(void)
{
}

std::wostream& NullVariant::format(std::wostream& stream) const
{
	stream << "NullVariant: Null";
	return stream;
}

double NullVariant::toFloating() const
{
	// Maybe throw?
	return 0.0;
}

__int64 NullVariant::toInteger64() const
{
	return 0;
}

int NullVariant::toInteger32() const
{
	return 0;
}

bool NullVariant::toBoolean() const
{
	return false;
}

shared_string NullVariant::toString() const
{
	return create_shared_string(L"Null");
}

bool NullVariant::equal(Variant* other)
{
	return other->isNullType();
}