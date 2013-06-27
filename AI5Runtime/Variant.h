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
	static const VARIANT_TYPE REFERENCE			= 0;
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
	static const int NUMBER_OF_VARIANT_TYPES	= 18;
	Variant(const VARIANT_TYPE type,bool container=false);
	virtual ~Variant(void);
	virtual std::wostream& format(std::wostream& stream) const;
	virtual double toFloating() const;
	virtual __int64 toInteger64() const;
	virtual int toInteger32() const;
	virtual bool toBoolean() const;
	virtual shared_string toString() const;
	virtual bool equal(Variant*);
	virtual void cleanup();
	virtual size_t hash() const;
	virtual IteratorVariant* iterate();
	__forceinline int addRef();
	__forceinline int release();
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
	inline bool isReferenceType() const;
	inline bool isNameType() const;
	inline bool isDefaultType() const;
	inline bool isNameReferenceType() const;
	inline bool isContainerType() const;
	inline bool isBinaryType() const;
	inline bool isHashMap() const;
	inline bool isIterator() const;

	static Variant* createFromCOMVar(const COMVar&);

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
	int refCount;
	bool isContainer;
	VariantFactory* recycler;
};


typedef struct
{
	size_t operator() (Variant* k) const
	{
		return k->hash();
	}
} VariantKeyHasher;
 
typedef struct
{
	bool operator() (Variant* x,Variant* y) const {
		return x->equal(y);
	}
} VariantKeyComparator;



class VariantFactory
{
public:
	VariantFactory(void);
	~VariantFactory(void);
	__forceinline void recycle(Variant* variant);

	// TODO: Infer VARIANT_TYPE from OUTER.
	template<typename OUTER,typename INNER>
	inline OUTER* create(VARIANT_TYPE type,INNER data);
	template<typename OUTER>
	inline OUTER* createEmpty(VARIANT_TYPE type);
private:
	static const int RECYCLE_BIN_LIMIT = 1000;
	std::vector<FastStack<Variant*>*> recycleBins;
};

void VariantFactory::recycle(Variant* variant)
{
	// Do have any place left in the recycle bin for the type??
	if(recycleBins[variant->getType()]->size() < RECYCLE_BIN_LIMIT)
	{
		// Store the variant for future use.
		recycleBins[variant->getType()]->push(variant);
	}
	else
	{
		// The limit for the bin has been used up!
		// Do a real delete.
		delete variant;
	}
}

template<typename OUTER,typename INNER>
OUTER* VariantFactory::create(VARIANT_TYPE type,INNER data)
{
	// Are there cached variants available?
	if(recycleBins[type]->size() > 0)
	{
		OUTER* var = static_cast<OUTER*>(recycleBins[type]->pop());

		// Prepare the recycled value for use.
		//var->value = data;
		var->setValue(data);
		var->addRef();

		return var;
	} else
	{
		// No old variant found! Create new and return it.
		OUTER* var = new OUTER(data);
		var->scheduleRecycling(this);
		return var;
	}
}

template<typename OUTER>
OUTER* VariantFactory::createEmpty(VARIANT_TYPE type)
{
	// Are there cached variants available?
	if(recycleBins[type]->size() > 0)
	{
		OUTER* var = static_cast<OUTER*>(recycleBins[type]->pop());
		var->addRef();

		return var;
	} else
	{
		// No old variant found! Create new and return it.
		OUTER* var = new OUTER();
		var->scheduleRecycling(this);
		return var;
	}
}

// Mark this object for recycling.
void Variant::scheduleRecycling(VariantFactory* factory)
{
	this->recycler = factory;
}

int Variant::addRef()
{
	return ++refCount;
}

int Variant::release()
{
	int resultCount = --refCount;
	if (!(resultCount))
	{
		this->cleanup();
		// Is this object used for recycling?
		if (recycler != nullptr)
		{
			recycler->recycle(this);
		}
		else
		{
			delete this;
		}
	}
	return resultCount;
}

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
bool Variant::isReferenceType() const
{
	return type == REFERENCE;
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

bool Variant::isContainerType() const
{
	return isContainer;
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