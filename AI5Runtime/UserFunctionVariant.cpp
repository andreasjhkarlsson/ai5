#include "UserFunctionVariant.h"
#include "StackMachineThread.h"

UserFunctionVariant::UserFunctionVariant(int address): address(address), FunctionVariant(TYPE), enclosingScope()
{


}


UserFunctionVariant::~UserFunctionVariant(void)
{
	
}

UserFunctionVariant* UserFunctionVariant::Create(int address)
{
	return GC::alloc<UserFunctionVariant,int>(address);
}

std::wostream& UserFunctionVariant::format(std::wostream& stream) const
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


VariantReference<Scope>& UserFunctionVariant::getEnclosingScope()
{
	return enclosingScope;
}

void UserFunctionVariant::setEnclosingScope(const VariantReference<Scope>& scope)
{
	this->enclosingScope = GC::persistReference(this,scope);
}