#include "BooleanVariant.h"
#include <iostream>

BooleanVariant::BooleanVariant(bool value): Variant(BOOLEAN), value(value)
{
}


BooleanVariant::~BooleanVariant(void)
{
	int a = 10;
}

BooleanVariant BooleanVariant::True(true);
BooleanVariant BooleanVariant::False(false);


double BooleanVariant::toFloating()
{
	if(value)
		return 1.0;
	return 0.0;
}

__int64 BooleanVariant::toInteger64()
{
	if(value)
		return 1;
	return 0;
}

int BooleanVariant::toInteger32()
{
	if(value)
		return 1;
	return 0;
}

bool BooleanVariant::toBoolean()
{
	return value;
}

std::wostream& BooleanVariant::format(std::wostream& stream)
{
	stream << "BooleanVariant: " << *toString();
	return stream;
}

shared_string BooleanVariant::toString()
{
	if(value)
		return create_shared_string(L"true");
	else
		return create_shared_string(L"false");
}

BooleanVariant* BooleanVariant::Get(bool value,bool increaseRefcount)
{
	BooleanVariant* res;
	if(value)
		res = &True;
	res = &False;
	if(increaseRefcount)
		res->addRef();
	return res;
}

bool BooleanVariant::equal(Variant* other)
{
	return getType() == other->getType() &&
			(value == static_cast<BooleanVariant*>(other)->value);
}