#pragma once
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include "Variant.h"
#include "NameVariant.h"
#include "PooledObject.h"

class StackMachine;

// Keeping this class optimized is VERY important.
class Scope: public PooledObject<Scope>
{
public:
	Scope(): indexTable(128,0),usedIndexes(),refCount(1),enclosingScope(nullptr)
	{
		usedIndexes.reserve(16);
	}
	__forceinline NameVariant* getNameFromString(const std::wstring &name)
	{
		return lookup[name];
	}
	__forceinline NameVariant* getNameFromIndex(int index)
	{
		NameVariant* result = indexTable[index];
		if(result == nullptr && enclosingScope != nullptr)
			result = enclosingScope->getNameFromIndex(index);
		return result;
	}
	NameVariant* createName(StackMachine* machine,const std::wstring &name);
	NameVariant* createIndexForName(StackMachine* machine,const std::wstring &name,int index);

	void insertName(const std::wstring& name,int index,NameVariant* nameVariant);

	void release();
	void addRef();

	void setEnclosingScope(Scope* scope)
	{
		if(scope != nullptr)
			scope->addRef();
		this->enclosingScope = scope;
	}

	static const int POOL_SIZE = 1024;
private:
	
	Scope* enclosingScope;

	int refCount;

	// Make the scope ready for reusing.
	void reset();

	// The string->name lookup.
	// All names in the scope NEEDS to be in this map.
	std::map<std::wstring,NameVariant*> lookup;

	// Used to provide super fast lookup of names in this scope.
	// Not all names are necessarily in this table. 
	std::vector<NameVariant*> indexTable;

	// List of used index in this table.
	// Used to avoid clearing the entire indexTable whenever a scope object is reused.
	std::vector<int> usedIndexes;

	void addNameToIndex(int index,NameVariant* nameVariant);
};