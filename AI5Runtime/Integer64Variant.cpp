#include "Integer64Variant.h"
#include <iostream>
#include "StackMachineThread.h"
#include <sstream>
#include "3rdparty\hsieh_hash.h"

Integer64Variant::Integer64Variant(const __int64 integer): Variant(TYPE), value(integer)
{
	//std::cout << "Creating new instance with value: " << integer << std::endl;
}

void Integer64Variant::setValue(__int64 value)
{
	this->value = value;
}

__int64 Integer64Variant::getValue()
{
	return value;
}

std::wostream& Integer64Variant::format(std::wostream& stream) const
{
	stream << "IntegerVariant64: " << toString()->getTerminatedBuffer();
	return stream;
}

__int64 Integer64Variant::toInteger64() const
{
	return value;
}

int Integer64Variant::toInteger32() const
{
	// LOSS OF DATA!!
	return (int)value;
}

bool Integer64Variant::toBoolean() const
{
	return value != 0;
}

double Integer64Variant::toFloating() const
{
	return (double)value;
}

Integer64Variant* Integer64Variant::createFromFactory(VariantFactory* factory,__int64 value)
{
	return factory->create<Integer64Variant,__int64>(Variant::INTEGER64,value);
}

shared_string Integer64Variant::toString() const
{
	std::wstringstream stream;
	stream << value;
	return create_shared_string(stream.str().c_str());
}


bool Integer64Variant::equal(Variant* other)
{
	return getType() == other->getType() &&
			(value == static_cast<Integer64Variant*>(other)->value);
}

size_t Integer64Variant::hash() const
{
	return SuperFastHash((const char*)&value,sizeof(value));
}