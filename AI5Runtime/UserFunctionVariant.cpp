#include "UserFunctionVariant.h"
#include "StackMachine.h"

UserFunctionVariant::UserFunctionVariant(int address): address(address), FunctionVariant(USER_FUNCTION)
{
}


UserFunctionVariant::~UserFunctionVariant(void)
{
}


void UserFunctionVariant::call(StackMachine* machine,int numberOfArguments)
{
	machine->pushCallFrame(numberOfArguments);

	machine->jumpAbsolute(address);


}

void UserFunctionVariant::print()
{
	std::wcout << "UserFunction @" << address << std::endl;
}