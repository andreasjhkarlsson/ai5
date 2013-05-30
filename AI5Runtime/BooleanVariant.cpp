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
void BooleanVariant::print()
{
	std::wcout << "BooleanVariant: " << *toString() << std::endl;
}

shared_string BooleanVariant::toString()
{
	if(value)
		return create_shared_string(L"true");
	else
		return create_shared_string(L"false");
}