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

template <class T>
class VariantReference
{
public:
	VariantReference(T*);
	VariantReference(void);
	VariantReference(int);
	VariantReference(double);
	VariantReference(__int64);
	VariantReference(bool);
	VariantReference(shared_string);
	VariantReference(const VariantReference<T>&);
	~VariantReference(void);
	static VariantReference<Variant> NullReference();
	static VariantReference<Variant> DefaultReference();
	VariantReference& operator= (const VariantReference<T>& other);

	template <typename U>
	VariantReference<U> cast() const
	{
		return VariantReference<U>(static_cast<U*>(ref.variant));
	}

	T* operator->()
	{
		return static_cast<T*>(ref.variant);
	}

	const T* operator->() const
	{
		return static_cast<const T*>(ref.variant);
	}

	T* get()
	{
		return static_cast<T*>(ref.variant);
	}

	// All VariantReferences can automatically be converted to generic type.
	operator VariantReference<>() const
	{
		return cast<Variant>();
	}

	int toInteger32() const;
	__int64 toInteger64() const;
	double toFloating() const;
	bool toBoolean() const;
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
private:

	bool isComplexType() const;

	// Certain datatypes are stored inline.
	union Reference
	{
		int int32;
		__int64 int64;
		double floating;
		bool boolean;
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
	if(isComplexType())
		ref.variant->release();
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
			variant->release();
			break;
		case Variant::INTEGER64:
			ref.int64 = variant->toInteger64();
			variant->release();
			break;
		case Variant::FLOATING:
			ref.floating = variant->toFloating();
			variant->release();
			break;
		case Variant::BOOLEAN:
			ref.boolean = variant->toBoolean();
			variant->release();
			break;
		case Variant::NULL_VAR:
			variant->release();
			break;
		case Variant::DEFAULT:
			variant->release();
			break;
		default:
			ref.variant = variant;
			ref.variant->addRef();
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
VariantReference<T>::VariantReference(shared_string str): varType(Variant::STRING)
{
	ref.variant = new StringVariant(str);
	ref.variant->addRef();
}


template <class T>
VariantReference<T>::VariantReference(bool value): varType(Variant::BOOLEAN)
{
	ref.boolean = value;
}

template <class T>
VariantReference<T>::VariantReference(const VariantReference& other): varType(other.varType), ref(other.ref)
{
	if(isComplexType())
		ref.variant->addRef();
}

template <class T>
VariantReference<T>& VariantReference<T>::operator=(const VariantReference<T>& other)
{
	if(isComplexType())
		this->ref.variant->release();
	this->varType = other.varType;
	this->ref = other.ref;
	if(isComplexType())
		this->ref.variant->addRef();
	return *this;
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
		return 0;
	case Variant::DEFAULT:
		return 0;
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
		return 0;
	case Variant::DEFAULT:
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
		return 0;
	case Variant::DEFAULT:
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
		return other.varType == Variant::NULL_VAR;
	case Variant::DEFAULT:
		return other.varType == Variant::DEFAULT;
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
				return ref.variant->equal(ref.boolean ? &BooleanVariant::True : &BooleanVariant::False);
			case Variant::NULL_VAR:
				return ref.variant->equal(&NullVariant::Instance);
			case Variant::DEFAULT:
				return ref.variant->equal(&DefaultVariant::Instance);
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
		return SuperFastHash((const char*)&NullVariant::Instance,sizeof(NullVariant));
	case Variant::DEFAULT:
		return SuperFastHash((const char*)&DefaultVariant::Instance,sizeof(DefaultVariant));
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
	default:
		return ref.variant->format(stream);
	}
	return stream;
}

