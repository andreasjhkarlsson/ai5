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
	std::wcout << "NullVariant: Null" << std::endl;
}

double NullVariant::toFloating()
{
	// Maybe throw?
	return 0.0;
}

__int64 NullVariant::toInteger64()
{
	return 0;
}

int NullVariant::toInteger32()
{
	return 0;
}

bool NullVariant::toBoolean()
{
	return false;
}

NullVariant NullVariant::Instance;

shared_string NullVariant::toString()
{
	return create_shared_string(L"Null");
}