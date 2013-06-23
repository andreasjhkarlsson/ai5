#include "BinaryVariant.h"
#include <sstream>
#include <string>
#include "3rdparty\hsieh_hash.h"

BinaryVariant::BinaryVariant(shared_binary binary): binary(binary), Variant(BINARY)
{
}


BinaryVariant::~BinaryVariant(void)
{
}


shared_binary BinaryVariant::getValue()
{
	return binary;
}


std::wostream& BinaryVariant::format(std::wostream& stream)
{
	stream << L"BinaryVariant: " << *toString();
	return stream;
}
bool BinaryVariant::toBoolean()
{
	return binary->size() > 0;
}
bool BinaryVariant::equal(Variant* other)
{
	return getType() == other->getType() && (*binary) == (*static_cast<BinaryVariant*>(other)->binary);
}


shared_string BinaryVariant::toString()
{
	std::wstringstream stream;
	for(int i=0;i<binary->size();i++)
	{
		stream << std::hex << (*binary)[i];
	}
	return shared_string(new std::wstring(stream.str()));
}

size_t BinaryVariant::hash()
{
	return SuperFastHash((const char*)&(*binary)[0],binary->size());
}