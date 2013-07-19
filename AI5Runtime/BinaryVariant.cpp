#include "BinaryVariant.h"
#include <sstream>
#include <string>
#include <iomanip>
#include "3rdparty\hsieh_hash.h"
#include "gc.h"

BinaryVariant::BinaryVariant(shared_binary binary): binary(binary), Variant(TYPE)
{
}


BinaryVariant::~BinaryVariant(void)
{
}


shared_binary BinaryVariant::getValue()
{
	return binary;
}


BinaryVariant* BinaryVariant::Create(shared_binary bin)
{

	return GC::alloc<BinaryVariant,shared_binary>(bin);

}

std::wostream& BinaryVariant::format(std::wostream& stream) const
{
	stream << L"BinaryVariant: " << toString()->getTerminatedBuffer();
	return stream;
}
bool BinaryVariant::toBoolean() const
{
	return binary->size() > 0;
}
bool BinaryVariant::equal(Variant* other)
{
	return getType() == other->getType() && (*binary) == (*static_cast<BinaryVariant*>(other)->binary);
}


shared_string BinaryVariant::toString() const
{
	std::wstringstream stream;
	for(int i=0;i<binary->size();i++)
	{
		stream << std::hex << std::setw(2) << std::setfill(L'0') << (unsigned char)(*binary)[i];
	}
	return shared_string(new UnicodeString(std::wstring(stream.str()).c_str()));
}

size_t BinaryVariant::hash() const
{
	return SuperFastHash((const char*)&(*binary)[0],binary->size());
}