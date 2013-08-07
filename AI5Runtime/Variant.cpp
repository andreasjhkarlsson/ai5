#include <iostream>
#include "GlobalOptions.h"
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
#include <iomanip>
#include "misc.h"

Variant::Variant(const VARIANT_TYPE type,bool verbosePrint): type(type), verbosePrint(verbosePrint)
{
	if (GlobalOptions::isVerbose())
	{
		if(verbosePrint)
		{
			DebugOut(L"Variant") << typeAsString() << ""
			<< "(0x"<< std::hex << std::setw(sizeof(void*)*2) << std::setfill(L'0') << (long)this << ") created.";
			std::wcout << std::dec << std::setw(0);
		}
	}
}

Variant::~Variant(void)
{
	if (GlobalOptions::isVerbose())
	{
		if(verbosePrint)
		{
			DebugOut(L"Variant") << typeAsString() << ""
			<< "(0x"<< std::hex << std::setw(sizeof(void*)*2) << std::setfill(L'0') << (long)this << ") destroyed.";
			std::wcout << std::dec << std::setw(0);
		}
	}
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


VariantReference<IteratorVariant> Variant::iterate()
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
	case VT_I1:
		return comvar.bVal;
	case VT_I2:
		return comvar.iVal;
	case VT_UI4:
		return (int)comvar.uintVal;
	case VT_I4:
		return comvar.intVal;
	case VT_I8:
		return comvar.llVal;
	case VT_R8:
		return comvar.dblVal;
	case VT_R4:
		return comvar.fltVal;
	case VT_BSTR:
		return StringVariant::Create(create_shared_string(comvar.bstrVal,SysStringLen(comvar.bstrVal)));
	case VT_EMPTY:
		return VariantReference<>::NullReference();
	default:
		throw RuntimeError(L"No conversion exists for COMVar");
	}
}

const char* Variant::typeAsString()
{
	switch(type)
	{
	case UNKNOWN:
		return "Unknown";
	case INTEGER32:
		return "Integer32";
	case INTEGER64:
		return "Integer64";
	case BOOLEAN:
		return "Boolean";
	case FLOATING:
		return "Floating";
	case NULL_VAR:
		return "Null";
	case NATIVE_FUNCTION:
		return "Native function";
	case USER_FUNCTION:
		return "User function";
	case STRING:
		return "String";
	case LIST:
		return "List";
	case NAME:
		return "Name";
	case NAME_REFERENCE:
		return "Name reference";
	case SCOPE:
		return "Scope";
	case BINARY:
		return "Binary";
	case HASH_MAP:
		return "Hash map";
	case ITERATOR:
		return "Iterator";
	case HANDLE_VAR:
		return "Handle";
	case DEFAULT:
		return "Default";
	case THREAD_VAR:
		return "Thread";
	default:
		return "ERROR TYPE";
	}
}