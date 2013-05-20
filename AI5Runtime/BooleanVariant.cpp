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
__int64 BooleanVariant::toInteger()
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
	if(value)
		std::wcout << "BooleanVariant: true" << std::endl;
	else
		std::wcout << "BooleanVariant: false" << std::endl;
}