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
__int64 FloatingVariant::toInteger64()
{
	return (__int64)value;
}

int FloatingVariant::toInteger32()
{
	return (int)value;
}

bool FloatingVariant::toBoolean()
{
	return value != 0.0;
}

std::wostream& FloatingVariant::format(std::wostream& stream)
{
	stream << "FloatingVariant: " << *toString();
	return stream;
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