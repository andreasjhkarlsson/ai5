#include "StringVariant.h"
#include <wchar.h>
#include <iostream>
#include <Windows.h>
#include "3rdparty\hsieh_hash.h"
#include "misc.h"

StringVariant::StringVariant(shared_string str): Variant(TYPE), str(str)
{

}

StringVariant::~StringVariant(void)
{

}

StringVariant* StringVariant::Create(shared_string str)
{
	return GC::alloc<StringVariant,shared_string>(str);
}

StringVariant* StringVariant::Create(const UnicodeString& str)
{
	return GC::alloc<StringVariant,shared_string>(create_shared_string(str));
}


StringVariant* StringVariant::CreateStatic(shared_string str)
{
	return GC::staticAlloc<StringVariant,shared_string>(str);
}

StringVariant* StringVariant::CreateStatic(const UnicodeString& str)
{
	return GC::staticAlloc<StringVariant,shared_string>(create_shared_string(str));
}


std::wostream& StringVariant::format(std::wostream& stream) const
{
	stream << "StringVariant: " << str->getTerminatedBuffer();
	return stream;

}
double StringVariant::toFloating() const
{
	return _wtof(str->getTerminatedBuffer());
}
__int64 StringVariant::toInteger64() const
{
	return _wtoi64(str->getTerminatedBuffer());
}


int StringVariant::toInteger32() const
{
	return _wtoi(str->getTerminatedBuffer());
}

bool StringVariant::toBoolean() const
{
	return str->length() > 0;
}

shared_string StringVariant::toString() const
{
	return str;
}

bool StringVariant::equal(Variant* other)
{
	return (getType() == other->getType()) &&
		(wcscmp(str->getTerminatedBuffer(),static_cast<StringVariant*>(other)->str->getTerminatedBuffer()) == 0);
}


size_t StringVariant::hash() const
{
	// Only hash first 32 chars.
	if(str->length() < 32)
		return SuperFastHash((const char*)str->getTerminatedBuffer(),str->length()*2);
	return SuperFastHash((const char*)str->getTerminatedBuffer(),32*2);
}