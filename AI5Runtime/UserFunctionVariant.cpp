#include "UserFunctionVariant.h"
#include "StackMachineThread.h"

UserFunctionVariant::UserFunctionVariant(int address): address(address), FunctionVariant(TYPE), enclosingScope()
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

void UserFunctionVariant::cleanup()
{
	//std::wcout << "Destroying function @" << address << std::endl;
	Variant::cleanup();
	
}


bool UserFunctionVariant::equal(Variant* other)
{
	return (getType() != other->getType()) &&
		(address == static_cast<UserFunctionVariant*>(other)->address);
}


VariantReference<Scope>& UserFunctionVariant::getEnclosingScope()
{
	return enclosingScope;
}

void UserFunctionVariant::setEnclosingScope(const VariantReference<Scope>& scope)
{
	this->enclosingScope = scope;
}