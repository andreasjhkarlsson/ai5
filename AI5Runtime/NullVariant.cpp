#include "NullVariant.h"
#include <iostream>

NullVariant::NullVariant(void): Variant(Variant::NULL_VAR)
{
}


NullVariant::~NullVariant(void)
{
}



void NullVariant::print()
{
	std::cout << "NullVariant: Null" << std::endl;
}

double NullVariant::toFloating()
{
	// Maybe throw?
	return 0.0;
}

__int64 NullVariant::toInteger()
{
	return 0;
}
bool NullVariant::toBoolean()
{
	return false;
}

NullVariant NullVariant::Instance;