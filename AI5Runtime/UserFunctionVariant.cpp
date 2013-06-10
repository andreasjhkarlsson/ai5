#include "UserFunctionVariant.h"
#include "StackMachine.h"

UserFunctionVariant::UserFunctionVariant(int address): address(address), FunctionVariant(USER_FUNCTION)
{
}


UserFunctionVariant::~UserFunctionVariant(void)
{
}

std::wostream& UserFunctionVariant::format(std::wostream& stream)
{
	stream << "UserFunction @" << address;
	return stream;
}


int UserFunctionVariant::getAddress()
{
	return address;
}


bool UserFunctionVariant::equal(Variant* other)
{
	return (getType() != other->getType()) &&
		(address == static_cast<UserFunctionVariant*>(other)->address);
}