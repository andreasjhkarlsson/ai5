#pragma once
#include <vector>
#include <iostream>
#include "FastStack.h"
#include "types.h"

class VariantFactory;
class NameVariant;

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
	static const int NUMBER_OF_VARIANT_TYPES	= 13;
	Variant(const VARIANT_TYPE type);
	virtual ~Variant(void);
	virtual std::wostream& format(std::wostream& stream)=0;
	virtual double toFloating()=0;
	virtual __int64 toInteger64()=0;
	virtual int toInteger32()=0;
	virtual bool toBoolean()=0;
	virtual shared_string toString()=0;
	virtual bool equal(Variant*)=0;
	virtual void cleanup();
	__forceinline void addRef();
	__forceinline void release();
	__forceinline VARIANT_TYPE getType();
	__forceinline void scheduleRecycling(VariantFactory* factory);

	inline bool isIntegerType();
	inline bool isInteger32Type();
	inline bool isInteger64Type();
	inline bool isFloatingType();
	inline bool isBooleanType();
	inline bool isNullType();
	inline bool isFunctionType();
	inline bool isNativeFunctionType();
	inline bool isUserFunctionType();
	inline bool isStringType();
	inline bool isListType();
	inline bool isReferenceType();
	inline bool isNameType();
	inline bool isDefaultType();
	inline bool isNameReferenceType();

private:
	const VARIANT_TYPE type;
	int refCount;
	VariantFactory* recycler;
};

class VariantFactory
{
public:
	VariantFactory(void);
	~VariantFactory(void);
	__forceinline void recycle(Variant* variant);

	// TODO: Infer VARIANT_TYPE from OUTER.
	template<typename OUTER,typename INNER>
	__forceinline OUTER* create(VARIANT_TYPE type,INNER data);
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

// Mark this object for recycling.
void Variant::scheduleRecycling(VariantFactory* factory)
{
	this->recycler = factory;
}

void Variant::addRef()
{
	refCount++;
}

void Variant::release()
{
	if (!(--refCount))
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
}

VARIANT_TYPE Variant::getType()
{
	return type;
}

bool Variant::isIntegerType()
{
	return type == INTEGER32 || type == INTEGER64;
}

bool Variant::isInteger32Type()
{
	return type == INTEGER32;
}
bool Variant::isInteger64Type()
{
	return type == INTEGER64;
}
bool Variant::isFloatingType()
{
	return type == FLOATING;
}
bool Variant::isBooleanType()
{
	return type == BOOLEAN;
}
bool Variant::isNullType()
{
	return type == NULL_VAR;
}
bool Variant::isFunctionType()
{
	return type == NATIVE_FUNCTION || type == USER_FUNCTION;
}
bool Variant::isNativeFunctionType()
{
	return type == NATIVE_FUNCTION;
}
bool Variant::isUserFunctionType()
{
	return type == USER_FUNCTION;
}
bool Variant::isStringType()
{
	return type == STRING;
}
bool Variant::isListType()
{
	return type == LIST;
}
bool Variant::isReferenceType()
{
	return type == REFERENCE;
}

bool Variant::isNameType()
{
	return type == NAME;
}

bool Variant::isNameReferenceType()
{
	return type == NAME_REFERENCE;
}

bool Variant::isDefaultType()
{
	return type == DEFAULT;
}