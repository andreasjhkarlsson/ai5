#include "ReferenceVariant.h"
#include <iostream>


void ReferenceVariant::print()
{
	std::cout << "ReferenceVariant: <" << this->var << ">" << std::endl;
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
__int64 ReferenceVariant::toInteger()
{
	// raise error
	return 0.0;
}
bool ReferenceVariant::toBoolean()
{
	return this->var != nullptr;
}