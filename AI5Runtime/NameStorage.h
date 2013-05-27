#pragma once

#include <vector>
#include <map>
#include <string>
#include "Variant.h"
#include "FastStack.h"
#include <algorithm>

// Represents a name.
class Name
{
public:
	Name(): variant(nullptr)
	{
	}
	virtual ~Name()
	{
	}

	__forceinline void set(Variant *newVariant)
	{
		newVariant->addRef();
		if(variant != nullptr)
			variant->release();
		variant = newVariant;
	}

	__forceinline Variant* get()
	{
		return variant;
	}

	__forceinline void clear()
	{
		variant->release();
		variant = nullptr;
	}

private:
	Variant* variant;
};


// Keeping this class optimized is VERY important.
class Scope
{
private:
	static const int POOL_SIZE = 64;
public:
	Scope(): indexTable(128,0),usedIndexes(), namePool(POOL_SIZE)
	{
		usedIndexes.reserve(16);
	}
	__forceinline Name* getNameFromString(const std::wstring &name)
	{
		return lookup[name];
	}
	__forceinline Name* getNameFromIndex(int index)
	{
		return indexTable[index];
	}
	__forceinline Name* createName(const std::wstring &name)
	{
		Name* n = new Name();
		lookup[name] = n;
		return n;

	}
	__forceinline Name* createIndexForName(const std::wstring &name,int index)
	{
		if (lookup.find(name) == lookup.end())
		{
			if(!namePool.empty())
				lookup[name] = namePool.pop();
			else
				lookup[name] = new Name();
		}

		if((index) >= indexTable.size())
		{
			indexTable.resize(index+1);
		}

		Name* nameObj = lookup[name];

		indexTable[index] = nameObj;

		usedIndexes.push_back(index);

		return nameObj;
	}

	// Make the scope ready for reusing.
	void reset()
	{
		// Clear all names in the lookup. Will release variants. 
		for(auto it=lookup.begin();it!=lookup.end();it++)
		{
			it->second->clear();

			// Is there room left in the name pool?
			// If it is, add released name into it.
			if(namePool.size() < POOL_SIZE)
			{
				namePool.push(it->second);
			}
			else
			{
				delete it->second;
			}
		}

		// Make sure the indexTable is all null's.
		for(int i=0;i<usedIndexes.size();i++)
		{
/*			// Is there room left in the name pool?
			// If it is, add released name into it.
			if(namePool.size() < POOL_SIZE)
			{
				namePool.push(indexTable[usedIndexes[i]]);
			}
			else
			{
				delete indexTable[usedIndexes[i]];
			}
			*/
			indexTable[usedIndexes[i]] = nullptr;
			
		}

		lookup.clear();
		usedIndexes.clear();
	}
private:
	// The string->name lookup.
	// All names in the scope NEEDS to be in this map.
	std::map<std::wstring,Name*> lookup;

	// Used to provide super fast lookup of names in this scope.
	// Not all names are necessarily in this table. 
	std::vector<Name*> indexTable;

	// List of used index in this table.
	// Used to avoid clearing the entire indexTable whenever a scope object is reused.
	std::vector<int> usedIndexes;

	// Keep a pool of names to avoid extensive heap alloc.
	FastStack<Name*> namePool;
};