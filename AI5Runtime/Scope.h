#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "Variant.h"
#include "NameVariant.h"
#include "PooledObject.h"
#include "types.h"
#include "gc.h"

class StackMachineThread;

// Keeping this class optimized is VERY important.
class Scope: public Variant
{
public:
	friend class GC;
	static const VARIANT_TYPE TYPE = SCOPE;

	static Scope* Create();

	Scope(): indexTable(128,VariantReference<NameVariant>()),usedIndexes(),enclosingScope(), Variant(TYPE)
	{
		usedIndexes.reserve(16);
	}

	~Scope()
	{
	}

	__forceinline VariantReference<NameVariant>& getNameFromString(const UnicodeString &name)
	{
		return lookup[name];
	}
	__forceinline VariantReference<NameVariant> getNameFromIndex(int index)
	{

		VariantReference<NameVariant> result = indexTable[index];
		if(result.empty() && !enclosingScope.empty())
			result = enclosingScope->getNameFromIndex(index);
		return result;
	}
	VariantReference<NameVariant> createName(const UnicodeString &name);
	VariantReference<NameVariant> createIndexForName(const UnicodeString &name,int index);

	void insertName(const UnicodeString& name,int index,VariantReference<NameVariant> nameVariant);

	void setEnclosingScope(VariantReference<Scope> scope)
	{
		this->enclosingScope = scope;
	}

private:

	VariantReference<Scope> enclosingScope;

	// The string->name lookup.
	// All names in the scope NEEDS to be in this map.
	std::unordered_map<UnicodeString,VariantReference<NameVariant>,UnicodeStringHasher,UnicodeStringComparator> lookup;

	// Used to provide super fast lookup of names in this scope.
	// Not all names are necessarily in this table. 
	std::vector<VariantReference<NameVariant>> indexTable;

	// List of used index in this table.
	// Used to avoid clearing the entire indexTable whenever a scope object is reused.
	std::vector<int> usedIndexes;

	void addNameToIndex(size_t index,const VariantReference<NameVariant>& nameVariant);
};