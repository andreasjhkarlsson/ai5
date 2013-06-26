#include <iostream>
#include "Variant.h"
#include "NameVariant.h"
#include "BooleanVariant.h"
#include "StringVariant.h"
#include "Integer32Variant.h"
#include "Integer64Variant.h"
#include "FloatingVariant.h"
#include "NullVariant.h"
#include "BinaryVariant.h"
#include "COMVar.h"

Variant::Variant(const VARIANT_TYPE type,bool isContainer): refCount(1), type(type), recycler(nullptr), isContainer(isContainer)
{
}

Variant::~Variant(void)
{
	
}

VariantFactory::VariantFactory(void)
{
	for(int i=0;i<Variant::NUMBER_OF_VARIANT_TYPES;i++)
	{
		recycleBins.push_back(new FastStack<Variant*>(VariantFactory::RECYCLE_BIN_LIMIT));
	}
}
VariantFactory::~VariantFactory(void)
{
}

void Variant::cleanup()
{
//	std::wcout << "Cleaning up ";
//	this->format(std::wcout);
//	std::wcout << std::endl;
}


std::wostream& Variant::format(std::wostream& stream) const
{
	stream << L"No formatting available";
	return stream;
}
double Variant::toFloating() const
{
	return 0.0;
}
__int64 Variant::toInteger64() const
{
	return 0;
}
int Variant::toInteger32() const
{
	return 0;
}
bool Variant::toBoolean() const
{
	return false;
}
shared_string Variant::toString() const
{
	return shared_string(new std::wstring(L""));
}
bool Variant::equal(Variant*)
{
	return false;
}

size_t Variant::hash() const
{
	// In the default implementation only exact
	// same instances have the same hash.
	return (size_t)this;
}

Variant* Variant::createFromCOMVar(const COMVar& comvar)
{
	switch(comvar.vt)
	{
	case VT_BOOL:
		return BooleanVariant::Get(comvar.boolVal == VARIANT_TRUE);
		break;
	case VT_I1:
		return new Integer32Variant(comvar.bVal);
		break;
	case VT_I2:
		return new Integer32Variant(comvar.iVal);
		break;
	case VT_I4:
		return new Integer32Variant(comvar.intVal);
		break;
	case VT_I8:
		return new Integer64Variant(comvar.llVal);
		break;
	case VT_R8:
		return new FloatingVariant(comvar.dblVal);
		break;
	case VT_R4:
		return new FloatingVariant(comvar.fltVal);
		break;
	case VT_BSTR:
		return new StringVariant(create_shared_string(comvar.bstrVal,SysStringLen(comvar.bstrVal)));
		break;
	default:
		// TODO: Implement all other types.
		break;
	}
}