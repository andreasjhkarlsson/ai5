#include "FloatingVariant.h"
#include <iostream>
#include <sstream>

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
	std::wcout << "FloatingVariant: " << *toString() << std::endl;
}


shared_string FloatingVariant::toString()
{
	std::wstringstream sstream;
	sstream << value;
	return create_shared_string(sstream.str());
}


FloatingVariant* FloatingVariant::createFromFactory(VariantFactory* factory,double value)
{
	return factory->create<FloatingVariant,double>(Variant::FLOATING,value);
}