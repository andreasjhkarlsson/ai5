#include "NameVariant.h"
#include <iostream>
#include "RuntimeError.h"

NameVariant::NameVariant(Variant* value): Variant(NAME), isConst(false)
{
	this->setValue(value);
}

NameVariant::~NameVariant(void)
{
	cleanup();
}

std::wostream& NameVariant::format(std::wostream& stream)
{
	stream << L"NameVariant";
	return stream;
}
double NameVariant::toFloating()
{
	return 0.0;
}
__int64 NameVariant::toInteger64()
{
	return 0;
}
int NameVariant::toInteger32()
{
	return 0;
}
bool NameVariant::toBoolean()
{
	return value != nullptr;
}
shared_string NameVariant::toString()
{
	return shared_string(new std::wstring(L""));

}
void NameVariant::cleanup()
{
	if(value != nullptr)
	{
		clearLastName();
		value->release();
		value = nullptr;
		isConst = false;
	}
}

Variant* NameVariant::getValue()
{
	return value;
}

bool NameVariant::isConstName()
{
	return isConst;
}

void NameVariant::setValue(Variant* newVariant)
{
	if(newVariant == nullptr)
	{
		value = nullptr;
		return;
	}
	if(isConst)
	{
		throw RuntimeError(L"Cannot set value of const name!");
	}

	clearLastName();

	newVariant->addRef();
	newVariant->setLastName(this);
	if(value != nullptr)
		value->release();
	value = newVariant;	
}

void NameVariant::markAsConst()
{
	// Cannot set name as const several times.
	if(isConst)
	{
		throw RuntimeError(L"Cannot set name as const multiple times!");
	}
	isConst = true;
}

NameVariant* NameVariant::createFromFactory(VariantFactory* factory)
{
	return factory->create<NameVariant,Variant*>(Variant::NAME,nullptr);
}

void NameVariant::clearLastName()
{
	if(value != nullptr && value->getLastName() == this)
	{
		value->setLastName(nullptr);
	}

}