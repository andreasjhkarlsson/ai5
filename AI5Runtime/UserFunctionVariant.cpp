#include "UserFunctionVariant.h"
#include "StackMachine.h"

UserFunctionVariant::UserFunctionVariant(int address,Scope* enclosingScope): address(address), FunctionVariant(USER_FUNCTION),
	enclosingScope(enclosingScope)
{
	if(enclosingScope != nullptr)
		enclosingScope->addRef();

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
	Variant::cleanup();
	if(enclosingScope != nullptr)
	{
		enclosingScope->release();
		enclosingScope = nullptr;
	}
}


bool UserFunctionVariant::equal(Variant* other)
{
	return (getType() != other->getType()) &&
		(address == static_cast<UserFunctionVariant*>(other)->address);
}


Scope* UserFunctionVariant::getEnclosingScope()
{
	return enclosingScope;
}