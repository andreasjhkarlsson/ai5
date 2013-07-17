#include "Integer32Variant.h"
#include <sstream>
#include <iostream>
#include "3rdparty\hsieh_hash.h"

Integer32Variant::Integer32Variant(int value): Variant(TYPE), value(value)
{
}


Integer32Variant::~Integer32Variant(void)
{
}

std::wostream& Integer32Variant::format(std::wostream& stream) const
{
	stream << "Integer32: " << value;
	return stream;

}



double Integer32Variant::toFloating() const
{
	return (double)value;
}
__int64 Integer32Variant::toInteger64() const
{
	return (__int64)value;
}
int Integer32Variant::toInteger32() const
{
	return value;
}
bool Integer32Variant::toBoolean() const
{
	return value != 0;
}
shared_string Integer32Variant::toString() const
{
	std::wstringstream stream;
	stream << value;
	return create_shared_string(stream.str().c_str());
}


bool Integer32Variant::equal(Variant* other)
{
	return getType() == other->getType() &&
			(value == static_cast<Integer32Variant*>(other)->value);
}

size_t Integer32Variant::hash() const
{
	return SuperFastHash((const char*)&value,sizeof(value));
}


int Integer32Variant::getValue()
{
	return value;
}

void Integer32Variant::setValue(int value)
{
	this->value = value;
}