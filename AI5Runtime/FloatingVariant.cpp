#include "FloatingVariant.h"
#include <iostream>

FloatingVariant::FloatingVariant(double value): Variant(FLOATING), value(value)
{
}


FloatingVariant::~FloatingVariant(void)
{
}


double FloatingVariant::toFloating()
{
	return value;
}
__int64 FloatingVariant::toInteger()
{
	return (__int64)value;
}
bool FloatingVariant::toBoolean()
{
	return value != 0.0;
}
void FloatingVariant::print()
{
	std::wcout << "FloatingVariant: " << value << std::endl;
}