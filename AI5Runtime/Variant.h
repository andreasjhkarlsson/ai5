#pragma once
#include <vector>
#include <iostream>
#include "FastStack.h"
#include "types.h"

class VariantFactory;
class NameVariant;
class IteratorVariant;
class COMVar;

typedef int VARIANT_TYPE;

class Variant
{
public:
	static const VARIANT_TYPE UNKNOWN			= 0;
	static const VARIANT_TYPE INTEGER64			= 1;
	static const VARIANT_TYPE BOOLEAN			= 2;
	static const VARIANT_TYPE FLOATING			= 3;
	static const VARIANT_TYPE NULL_VAR			= 4;
	static const VARIANT_TYPE NATIVE_FUNCTION	= 5;
	static const VARIANT_TYPE USER_FUNCTION		= 6;
	static const VARIANT_TYPE STRING			= 7;
	static const VARIANT_TYPE LIST				= 8;
	static const VARIANT_TYPE INTEGER32			= 9;
	static const VARIANT_TYPE NAME				= 10;
	static const VARIANT_TYPE DEFAULT			= 11;
	static const VARIANT_TYPE NAME_REFERENCE	= 12;
	static const VARIANT_TYPE SCOPE				= 13;
	static const VARIANT_TYPE BINARY			= 14;
	static const VARIANT_TYPE HASH_MAP			= 15;
	static const VARIANT_TYPE ITERATOR			= 16;
	static const VARIANT_TYPE HANDLE_VAR		= 17;
	static const VARIANT_TYPE THREAD_VAR		= 18;
	static const int NUMBER_OF_VARIANT_TYPES	= 19;
	const char* typeAsString();


	Variant(const VARIANT_TYPE type);
	virtual ~Variant(void);
	virtual std::wostream& format(std::wostream& stream) const;
	virtual double toFloating() const;
	virtual __int64 toInteger64() const;
	virtual int toInteger32() const;
	virtual bool toBoolean() const;
	virtual shared_string toString() const;
	virtual bool equal(Variant*);
	virtual size_t hash() const;
	virtual VariantReference<IteratorVariant> iterate();
	__forceinline VARIANT_TYPE getType() const;
	__forceinline void scheduleRecycling(VariantFactory* factory);

	inline bool isIntegerType() const;
	inline bool isInteger32Type() const;
	inline bool isInteger64Type() const;
	inline bool isFloatingType() const;
	inline bool isBooleanType() const;
	inline bool isNullType() const;
	inline bool isFunctionType() const;
	inline bool isNativeFunctionType() const;
	inline bool isUserFunctionType() const;
	inline bool isStringType() const;
	inline bool isListType() const;
	inline bool isNameType() const;
	inline bool isDefaultType() const;
	inline bool isNameReferenceType() const;
	inline bool isBinaryType() const;
	inline bool isHashMap() const;
	inline bool isIterator() const;

	static VariantReference<> createFromCOMVar(const COMVar&);

	template<class T>
	T* cast()
	{
		if(getType() != T::TYPE)
		{
			// TODO: Nicer error.
			throw RuntimeError(L"Expected variant of type X got Y");
		}
		return static_cast<T*>(this);
	}

private:
	const VARIANT_TYPE type;
};


typedef struct
{
	size_t operator() (const VariantReference<>& k) const;
} VariantKeyHasher;
 
typedef struct
{
	bool operator() (const VariantReference<>& x,const VariantReference<>& y) const;
} VariantKeyComparator;


VARIANT_TYPE Variant::getType() const
{
	return type;
}

bool Variant::isIntegerType() const
{
	return type == INTEGER32 || type == INTEGER64;
}

bool Variant::isInteger32Type() const
{
	return type == INTEGER32;
}
bool Variant::isInteger64Type() const
{
	return type == INTEGER64;
}
bool Variant::isFloatingType() const
{
	return type == FLOATING;
}
bool Variant::isBooleanType() const
{
	return type == BOOLEAN;
}
bool Variant::isNullType() const
{
	return type == NULL_VAR;
}
bool Variant::isFunctionType() const
{
	return type == NATIVE_FUNCTION || type == USER_FUNCTION;
}
bool Variant::isNativeFunctionType() const
{
	return type == NATIVE_FUNCTION;
}
bool Variant::isUserFunctionType() const
{
	return type == USER_FUNCTION;
}
bool Variant::isStringType() const
{
	return type == STRING;
}
bool Variant::isListType() const
{
	return type == LIST;
}

bool Variant::isNameType() const
{
	return type == NAME;
}

bool Variant::isNameReferenceType() const
{
	return type == NAME_REFERENCE;
}

bool Variant::isDefaultType() const
{
	return type == DEFAULT;
}

bool Variant::isBinaryType() const
{
	return type == BINARY;
}

bool Variant::isHashMap() const
{
	return type == HASH_MAP;
}

bool Variant::isIterator() const
{
	return type == ITERATOR;
}