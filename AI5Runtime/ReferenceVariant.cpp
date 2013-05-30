#include "ReferenceVariant.h"
#include <iostream>
#include <sstream>

void ReferenceVariant::print()
{
	std::wcout << "ReferenceVariant: <" << *toString() << ">" << std::endl;
}

shared_string ReferenceVariant::toString()
{
	std::wstringstream stream;
	stream << var;
	return create_shared_string(stream.str());
}

ReferenceVariant::ReferenceVariant(Variant* var): Variant(REFERENCE), var(nullptr)
{
	this->assign(var);
}

ReferenceVariant::~ReferenceVariant()
{
	this->unlink();
}

void ReferenceVariant::assign(Variant* var)
{
	this->unlink();
	this->var = var;
	this->var->addRef();
}
void ReferenceVariant::unlink()
{
	if (var)
	{
		var->release();
		var = nullptr;
	}
}

double ReferenceVariant::toFloating()
{
	// raise error.
	return 0.0;

}

__int64 ReferenceVariant::toInteger64()
{
	// raise error
	return 0;
}

int ReferenceVariant::toInteger32()
{
	// raise error
	return 0;
}

bool ReferenceVariant::toBoolean()
{
	return this->var != nullptr;
}