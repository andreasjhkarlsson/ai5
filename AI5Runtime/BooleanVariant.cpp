#include "BooleanVariant.h"
#include <iostream>

BooleanVariant::BooleanVariant(bool value): Variant(TYPE), value(value)
{
}


BooleanVariant::~BooleanVariant(void)
{
	int a = 10;
}



double BooleanVariant::toFloating() const
{
	if(value)
		return 1.0;
	return 0.0;
}

__int64 BooleanVariant::toInteger64() const
{
	if(value)
		return 1;
	return 0;
}

int BooleanVariant::toInteger32() const
{
	if(value)
		return 1;
	return 0;
}

bool BooleanVariant::toBoolean() const
{
	return value;
}

std::wostream& BooleanVariant::format(std::wostream& stream) const
{
	stream << "BooleanVariant: " << toString()->getTerminatedBuffer();
	return stream;
}

shared_string BooleanVariant::toString() const
{
	if(value)
		return create_shared_string(L"true");
	else
		return create_shared_string(L"false");
}

bool BooleanVariant::equal(Variant* other)
{
	return getType() == other->getType() &&
			(value == static_cast<BooleanVariant*>(other)->value);
}