#include "IntegerVariant.h"
#include <iostream>
#include "StackMachine.h"
#include <sstream>

IntegerVariant::IntegerVariant(__int64 integer): Variant(INTEGER), value(integer)
{
	//std::cout << "Creating new instance with value: " << integer << std::endl;
}

IntegerVariant** IntegerVariant::BYTE_TABLE;
bool IntegerVariant::INITIALIZED = IntegerVariant::INITIALIZE();


bool IntegerVariant::INITIALIZE()
{
	BYTE_TABLE = new IntegerVariant*[256];
	for(short value=-128;value<=127;value++)
	{
		BYTE_TABLE[value+128] = new IntegerVariant(value);
	}
	return true;
}

void IntegerVariant::print()
{
	std::wcout << "IntegerVariant: " << *toString() << std::endl;
}

__int64 IntegerVariant::toInteger()
{
	return value;
}

bool IntegerVariant::toBoolean()
{
	return value != 0;
}

double IntegerVariant::toFloating()
{
	return (double)value;
}

IntegerVariant* IntegerVariant::createFromFactory(VariantFactory* factory,__int64 value)
{
	return factory->create<IntegerVariant,__int64>(Variant::INTEGER,value);
}

shared_string IntegerVariant::toString()
{
	std::wstringstream stream;
	stream << value;
	return create_shared_string(stream.str());
}