#include "HandleVariant.h"
#include "RuntimeError.h"

HandleVariant::HandleVariant(HANDLE_TYPE handleType): Variant(TYPE), handleType(handleType)
{
}


HandleVariant::~HandleVariant(void)
{
}


bool HandleVariant::toBoolean() const
{
	return isValid();
}

std::wostream& HandleVariant::format(std::wostream& stream) const
{
	stream << L"HandleVariant: <0x" << this << ">";
	return stream;
}