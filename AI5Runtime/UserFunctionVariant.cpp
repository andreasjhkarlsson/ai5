#include "UserFunctionVariant.h"
#include "StackMachine.h"

UserFunctionVariant::UserFunctionVariant(int address): address(address)
{
}


UserFunctionVariant::~UserFunctionVariant(void)
{
}


void UserFunctionVariant::call(StackMachine* machine)
{
	machine->pushCallFrame(machine->getCurrentAddress()+1);

	machine->jumpAbsolute(address);


}

void UserFunctionVariant::print()
{
	std::cout << "UserFunction @" << address << std::endl;
}