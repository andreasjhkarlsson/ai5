#include "Integer32Variant.h"
#include <sstream>
#include <iostream>
#include "3rdparty\hsieh_hash.h"

Integer32Variant::Integer32Variant(int value): Variant(INTEGER32), value(value)
{
}


Integer32Variant::~Integer32Variant(void)
{
}

std::wostream& Integer32Variant::format(std::wostream& stream)
{
	stream << "Integer32: " << value;
	return stream;

}

Integer32Variant* Integer32Variant::createFromFactory(VariantFactory* factory,int value)
{
	return factory->create<Integer32Variant,int>(Variant::INTEGER32,value);
}

double Integer32Variant::toFloating()
{
	return (double)value;
}
__int64 Integer32Variant::toInteger64()
{
	return (__int64)value;
}
int Integer32Variant::toInteger32()
{
	return value;
}
bool Integer32Variant::toBoolean()
{
	return value != 0;
}
shared_string Integer32Variant::toString()
{
	std::wstringstream stream;
	stream << value;
	return create_shared_string(stream.str());
}


bool Integer32Variant::equal(Variant* other)
{
	return getType() == other->getType() &&
			(value == static_cast<Integer32Variant*>(other)->value);
}

size_t Integer32Variant::hash()
{
	return SuperFastHash((const char*)&value,sizeof(value));
}