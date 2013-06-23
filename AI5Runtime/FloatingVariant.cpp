#include "FloatingVariant.h"
#include <iostream>
#include <sstream>
#include "3rdparty\hsieh_hash.h"

FloatingVariant::FloatingVariant(double value): Variant(FLOATING), value(value)
{
}


FloatingVariant::~FloatingVariant(void)
{
}


double FloatingVariant::toFloating() const
{
	return value;
}
__int64 FloatingVariant::toInteger64() const
{
	return (__int64)value;
}

int FloatingVariant::toInteger32() const
{
	return (int)value;
}

bool FloatingVariant::toBoolean() const
{
	return value != 0.0;
}

std::wostream& FloatingVariant::format(std::wostream& stream) const
{
	stream << "FloatingVariant: " << *toString();
	return stream;
}


shared_string FloatingVariant::toString() const
{
	std::wstringstream sstream;
	sstream << value;
	return create_shared_string(sstream.str());
}


FloatingVariant* FloatingVariant::createFromFactory(VariantFactory* factory,double value)
{
	return factory->create<FloatingVariant,double>(Variant::FLOATING,value);
}


bool FloatingVariant::equal(Variant* other)
{
	return getType() == other->getType() &&
			(value == static_cast<FloatingVariant*>(other)->value);
}

size_t FloatingVariant::hash() const
{
	return SuperFastHash((const char*)&value,sizeof(value));
}