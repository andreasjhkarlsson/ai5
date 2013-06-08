#include "UserFunctionVariant.h"
#include "StackMachine.h"

UserFunctionVariant::UserFunctionVariant(int address): address(address), FunctionVariant(USER_FUNCTION)
{
}


UserFunctionVariant::~UserFunctionVariant(void)
{
}

void UserFunctionVariant::print()
{
	std::wcout << "UserFunction @" << address << std::endl;
}


int UserFunctionVariant::getAddress()
{
	return address;
}