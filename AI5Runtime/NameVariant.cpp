#include "NameVariant.h"
#include <iostream>
#include "RuntimeError.h"

NameVariant::NameVariant(Variant* value,VARIANT_TYPE type): ContainerVariant(type), isConst(false),value(value)
{
	this->setValue(value);
}

NameVariant::~NameVariant(void)
{
	cleanup();
}

std::wostream& NameVariant::format(std::wostream& stream) const
{
	stream << L"NameVariant ";
	if(value != nullptr)
		value->format(stream);
	return stream;
}
double NameVariant::toFloating() const
{
	return 0.0;
}
__int64 NameVariant::toInteger64() const
{
	return 0;
}
int NameVariant::toInteger32() const
{
	return 0;
}
bool NameVariant::toBoolean() const
{
	return value != nullptr;
}
shared_string NameVariant::toString() const
{
	return shared_string(new UnicodeString(L""));

}
void NameVariant::cleanup()
{
	Variant::cleanup();
	if(value != nullptr)
	{
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

	newVariant->addRef();
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


bool NameVariant::equal(Variant* other)
{
	// Only compare equal for exactly the same instance.
	return this == other;
}


int NameVariant::getChildContainersCount()
{
	if(value->isContainerType())
		return 1;
	return 0;
}
ContainerVariant* NameVariant::getChildContainer(int index)
{
	// Assume that this method is only called if getChildContainersCount
	// returns 1 and that index is 0.
	return static_cast<ContainerVariant*>(value);
}