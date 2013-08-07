#pragma once
#include "Variant.h"
#include "types.h"
#include "3rdparty\hsieh_hash.h"
#include "BooleanVariant.h"
#include "Integer32Variant.h"
#include "Integer64Variant.h"
#include "FloatingVariant.h"
#include "NullVariant.h"
#include "DefaultVariant.h"
#include "ListVariant.h"
#include <sstream>
#include "gc.h"

template <class T>
class VariantReference
{
public:
	friend class GC;
	VariantReference(T*);
	VariantReference(void);
	VariantReference(int);
	VariantReference(double);
	VariantReference(__int64);
	VariantReference(bool);
	VariantReference(const VariantReference<T>&);
	~VariantReference(void);
	static VariantReference<Variant> NullReference();
	static VariantReference<Variant> DefaultReference();
	static VariantReference<Variant> PointerReference(void*);
	VariantReference& operator= (const VariantReference<T>& other);

	template <typename U>
	VariantReference<U> cast() const
	{
		if(this->varType != U::TYPE && U::TYPE != Variant::GENERIC)
		{
			throw RuntimeError(UnicodeString(L"Expected ")+Variant::typeAsString(U::TYPE)+L", but got "+Variant::typeAsString(varType));
		}
		return VariantReference<U>(static_cast<U*>(ref.variant));
	}

	T* operator->()
	{
		if(!this->isComplexType())
			throw RuntimeError(L"Pointer to non-complex type not allowed");
		return static_cast<T*>(ref.variant);
	}

	const T* operator->() const
	{
		if(!this->isComplexType())
			throw RuntimeError(L"Pointer to non-complex type not allowed");
		return static_cast<const T*>(ref.variant);
	}

	T* get()
	{
		if(!this->isComplexType())
			throw RuntimeError(L"Pointer to non-complex type not allowed");
		return static_cast<T*>(ref.variant);
	}

	const T* get() const
	{
		if(!this->isComplexType())
			throw RuntimeError(L"Pointer to non-complex type not allowed");
		return static_cast<T*>(ref.variant);
	}

	// All VariantReferences can automatically be converted to generic type.
	operator VariantReference<>() const
	{
		return cast<Variant>();
	}


	void clear()
	{
		varType = Variant::UNKNOWN;
	}

	int toInteger32() const;
	__int64 toInteger64() const;
	double toFloating() const;
	bool toBoolean() const;
	void* toPointer() const;
	shared_string toString() const;
	bool equal(const VariantReference& other) const;
	size_t hash() const;
	bool empty() const
	{
		return varType == Variant::UNKNOWN;
	}
	std::wostream& format(std::wostream& stream) const;

	VARIANT_TYPE getType() const
	{
		return varType;
	}

	bool isIntegerType() const
	{
		return varType == Variant::INTEGER32 || varType == Variant::INTEGER64;
	}

	bool isInteger32Type() const
	{
		return varType == Variant::INTEGER32;
	}
	bool isInteger64Type() const
	{
		return varType == Variant::INTEGER64;
	}
	bool isFloatingType() const
	{
		return varType == Variant::FLOATING;
	}
	bool isBooleanType() const
	{
		return varType == Variant::BOOLEAN;
	}
	bool isNullType() const
	{
		return varType == Variant::NULL_VAR;
	}
	bool isFunctionType() const
	{
		return varType == Variant::NATIVE_FUNCTION || varType == Variant::USER_FUNCTION;
	}
	bool isNativeFunctionType() const
	{
		return varType == Variant::NATIVE_FUNCTION;
	}
	bool isUserFunctionType() const
	{
		return varType == Variant::USER_FUNCTION;
	}
	bool isStringType() const
	{
		return varType == Variant::STRING;
	}
	bool isListType() const
	{
		return varType == Variant::LIST;
	}

	bool isNameType() const
	{
		return varType == Variant::NAME;
	}

	bool isNameReferenceType() const
	{
		return varType == Variant::NAME_REFERENCE;
	}

	bool isDefaultType() const
	{
		return varType == Variant::DEFAULT;
	}

	bool isBinaryType() const
	{
		return varType == Variant::BINARY;
	}

	bool isHashMap() const
	{
		return varType == Variant::HASH_MAP;
	}

	bool isIterator() const
	{
		return varType == Variant::ITERATOR;
	}

	bool isPointer() const
	{
		return varType == Variant::POINTER;
	}

private:

	bool isComplexType() const;

	// Certain datatypes are stored inline.
	union Reference
	{
		int int32;
		__int64 int64;
		double floating;
		bool boolean;
		void* pointer;
		Variant* variant;
	} ref;
	VARIANT_TYPE varType;
};

template <class T>
VariantReference<T>::VariantReference(void): varType(Variant::UNKNOWN)
{
	ref.variant = nullptr;
}

template <class T>
VariantReference<T>::~VariantReference(void)
{

}

template <class T>
VariantReference<T>::VariantReference(T* variant)
{
	if(variant == nullptr)
	{
		varType = Variant::NULL_VAR;
	}
	else
	{
		varType = variant->getType();
		switch(varType)
		{
		case Variant::INTEGER32:
			ref.int32 = variant->toInteger32();
			break;
		case Variant::INTEGER64:
			ref.int64 = variant->toInteger64();
			break;
		case Variant::FLOATING:
			ref.floating = variant->toFloating();
			break;
		case Variant::BOOLEAN:
			ref.boolean = variant->toBoolean();
			break;
		case Variant::NULL_VAR:
			break;
		case Variant::DEFAULT:
			break;
		default:
			ref.variant = variant;
			break;
		}
	}
}

template <class T>
bool VariantReference<T>::isComplexType() const
{
	switch(varType)
	{
	case Variant::INTEGER32:
	case Variant::INTEGER64:
	case Variant::FLOATING:
	case Variant::BOOLEAN:
	case Variant::NULL_VAR:
	case Variant::UNKNOWN:
	case Variant::DEFAULT:
	case Variant::POINTER:
		return false;
	}
	return true;
}

template <class T>
VariantReference<Variant> VariantReference<T>::NullReference()
{
	return VariantReference(nullptr);
}

template <class T>
VariantReference<Variant> VariantReference<T>::DefaultReference()
{
	VariantReference ref;
	ref.varType = Variant::DEFAULT;
	return ref;
}

template <class T>
VariantReference<Variant> VariantReference<T>::PointerReference(void* pointer)
{
	VariantReference<> ref;
	ref.varType = Variant::POINTER;
	ref.ref.pointer = pointer;
	return ref;
}

template <class T>
VariantReference<T>::VariantReference(int value): varType(Variant::INTEGER32)
{
	ref.int32 = value;
}

template <class T>
VariantReference<T>::VariantReference(double value): varType(Variant::FLOATING)
{
	ref.floating = value;
}

template <class T>
VariantReference<T>::VariantReference(__int64 value): varType(Variant::INTEGER64)
{
	ref.int64 = value;
}

template <class T>
VariantReference<T>::VariantReference(bool value): varType(Variant::BOOLEAN)
{
	ref.boolean = value;
}

template <class T>
VariantReference<T>::VariantReference(const VariantReference& other): varType(other.varType), ref(other.ref)
{

}

template <class T>
VariantReference<T>& VariantReference<T>::operator=(const VariantReference<T>& other)
{
	this->varType = other.varType;
	this->ref = other.ref;
	return *this;
}


template <class T>
void* VariantReference<T>::toPointer() const
{
	if(varType != Variant::POINTER)
	{
		throw RuntimeError(L"Variant is not of pointer type!");
	}
	return ref.pointer;
}

template <class T>
int VariantReference<T>::toInteger32() const
{
	switch(varType)
	{
	case Variant::INTEGER32:
		return ref.int32;
	case Variant::INTEGER64:
		return (int)ref.int64;
	case Variant::FLOATING:
		return (int)ref.floating;
	case Variant::BOOLEAN:
		return ref.boolean ? 1 : 0;
	case Variant::NULL_VAR:
	case Variant::DEFAULT:
	case Variant::POINTER:
		return 0; // Hah! No pointer arithmetic for you!
	default:
		return ref.variant->toInteger32();
	}
}

template <class T>
__int64 VariantReference<T>::toInteger64() const
{
	switch(varType)
	{
	case Variant::INTEGER32:
		return ref.int32;
	case Variant::INTEGER64:
		return ref.int64;
	case Variant::FLOATING:
		return (__int64)ref.floating;
	case Variant::BOOLEAN:
		return ref.boolean ? 1 : 0;
	case Variant::NULL_VAR:
	case Variant::DEFAULT:
	case Variant::POINTER:
		return 0;
	default:
		return ref.variant->toInteger64();
	}
}

template <class T>
double VariantReference<T>::toFloating() const
{
	switch(varType)
	{
	case Variant::INTEGER32:
		return ref.int32;
	case Variant::INTEGER64:
		return (double)ref.int64;
	case Variant::FLOATING:
		return ref.floating;
	case Variant::BOOLEAN:
		return ref.boolean ? 1 : 0;
	case Variant::NULL_VAR:
	case Variant::DEFAULT:
	case Variant::POINTER:
		return 0;
	default:
		return ref.variant->toFloating();
	}
}

template <class T>
bool VariantReference<T>::toBoolean() const
{
	switch(varType)
	{
	case Variant::INTEGER32:
		return ref.int32 != 0;
	case Variant::INTEGER64:
		return ref.int64 != 0;
	case Variant::FLOATING:
		return ref.floating != 0.0;
	case Variant::BOOLEAN:
		return ref.boolean;
	case Variant::NULL_VAR:
		return false;
	case Variant::DEFAULT:
		return false;
	case Variant::POINTER:
		return ref.pointer != nullptr;
	default:
		return ref.variant->toBoolean();
	}
}

template <class T>
shared_string VariantReference<T>::toString() const
{
	std::wstringstream sstream;
	switch(varType)
	{
	case Variant::BOOLEAN:
		return ref.boolean ? create_shared_string(L"True") : create_shared_string(L"False");
	case Variant::INTEGER32:
		sstream << ref.int32;
		break;
	case Variant::INTEGER64:
		sstream << ref.int64;
		break;
	case Variant::FLOATING:
		sstream << ref.floating;
		break;
	case Variant::NULL_VAR:
		return create_shared_string(L"Null");
	case Variant::DEFAULT:
		return create_shared_string(L"Default");
	case Variant::POINTER:
		sstream << ref.pointer;
		break;
	default:
		return ref.variant->toString();
	}
	return create_shared_string_from_wstring(sstream.str());
}

template <class T>
bool VariantReference<T>::equal(const VariantReference<T>& other) const
{
	switch(varType)
	{
	case Variant::INTEGER32:
		return ref.int32  == other.toInteger32();
	case Variant::INTEGER64:
		return ref.int64 == other.toInteger64();
	case Variant::FLOATING:
		return ref.floating == other.toFloating();
	case Variant::BOOLEAN:
		return ref.boolean == other.toBoolean();
	case Variant::NULL_VAR:
		if(other.varType == Variant::INTEGER32 || other.varType == Variant::INTEGER64)
			return other.toInteger32() == 0;
		return other.varType == Variant::NULL_VAR;
	case Variant::DEFAULT:
		return other.varType == Variant::DEFAULT;
	case Variant::POINTER:
		return reinterpret_cast<__int64>(ref.pointer) == other.toInteger64(); // Lazy! pointer always fit inside int64.
	default:
		{
			switch(other.varType)
			{
			case Variant::INTEGER32:
				return ref.variant->equal(&Integer32Variant(other.ref.int32));
			case Variant::INTEGER64:
				return ref.variant->equal(&Integer64Variant(other.ref.int64));
			case Variant::FLOATING:
				return ref.variant->equal(&FloatingVariant(other.ref.floating));
			case Variant::BOOLEAN:
				return ref.variant->equal(&BooleanVariant(ref.boolean));
			case Variant::NULL_VAR:
				return ref.variant->equal(&NullVariant());
			case Variant::DEFAULT:
				return ref.variant->equal(&DefaultVariant());
			case Variant::POINTER:
				return ref.variant->equal(&Integer64Variant(reinterpret_cast<__int64>(other.ref.pointer)));
			default:
				return ref.variant->equal(other.ref.variant);
			}
		}
	}
}

template <class T>
size_t VariantReference<T>::hash() const
{
	switch(varType)
	{
	case Variant::INTEGER32:
		return SuperFastHash((const char*)&ref.int32,sizeof(int));
	case Variant::INTEGER64:
		return SuperFastHash((const char*)&ref.int64,sizeof(__int64));
	case Variant::FLOATING:
		return SuperFastHash((const char*)&ref.floating,sizeof(double));
	case Variant::BOOLEAN:
		return SuperFastHash((const char*)&ref.boolean,sizeof(bool));
	case Variant::NULL_VAR:
		return SuperFastHash((const char*)&NullVariant(),sizeof(NullVariant));
	case Variant::DEFAULT:
		return SuperFastHash((const char*)&DefaultVariant(),sizeof(DefaultVariant));
	case Variant::POINTER:
		return SuperFastHash((const char*)&ref.pointer,sizeof(void*));
	default:
		return ref.variant->hash();
	}
}

template <class T>
std::wostream& VariantReference<T>::format(std::wostream& stream) const
{
	switch(varType)
	{
	case Variant::INTEGER32:
		stream << ref.int32;
		break;
	case Variant::INTEGER64:
		stream << ref.int64;
		break;
	case Variant::FLOATING:
		stream << ref.floating;
		break;
	case Variant::BOOLEAN:
		stream << ref.boolean;
		break;
	case Variant::NULL_VAR:
		stream << "Null";
		break;
	case Variant::DEFAULT:
		stream << "Default";
		break;
	case Variant::POINTER:
		stream << "Pointer: " << ref.pointer;
		break;
	default:
		return ref.variant->format(stream);
	}
	return stream;
}

