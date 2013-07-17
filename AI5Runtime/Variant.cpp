#include <iostream>
#include "Variant.h"
#include "NameVariant.h"
#include "IteratorVariant.h"
#include "BooleanVariant.h"
#include "StringVariant.h"
#include "Integer32Variant.h"
#include "Integer64Variant.h"
#include "FloatingVariant.h"
#include "NullVariant.h"
#include "BinaryVariant.h"
#include "COMVar.h"
#include "VariantReference.h"

Variant::Variant(const VARIANT_TYPE type,bool isContainer): refCount(0), type(type), isContainer(isContainer)
{
}

Variant::~Variant(void)
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
	return shared_string(new UnicodeString(L""));
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


IteratorVariant* Variant::iterate()
{
	throw RuntimeError(L"Type not iterable");
}



size_t VariantKeyHasher::operator() (const VariantReference<>& k) const
{
	return k.hash();
}

 

bool VariantKeyComparator::operator() (const VariantReference<>& x,const VariantReference<>& y) const
{
	return x.equal(y);
}



VariantReference<> Variant::createFromCOMVar(const COMVar& comvar)
{
	switch(comvar.vt)
	{
	case VT_BOOL:
		return comvar.boolVal != VARIANT_FALSE;
		break;
	case VT_I1:
		return comvar.bVal;
		break;
	case VT_I2:
		return comvar.iVal;
		break;
	case VT_I4:
		return comvar.intVal;
		break;
	case VT_I8:
		return comvar.llVal;
		break;
	case VT_R8:
		return comvar.dblVal;
		break;
	case VT_R4:
		return comvar.fltVal;
		break;
	case VT_BSTR:
		return new StringVariant(create_shared_string(comvar.bstrVal,SysStringLen(comvar.bstrVal)));
		break;
	case VT_EMPTY:
		return VariantReference<>::NullReference();
	default:
		throw RuntimeError(L"No conversion exists for COMVar");
		break;
	}
}