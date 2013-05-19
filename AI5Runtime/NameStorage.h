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


// Ok, here goes:
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
	__forceinline Name* getNameFromString(const std::string &name)
	{
		return lookup[name];
	}
	__forceinline Name* getNameFromIndex(int index)
	{
		return indexTable[index];
	}
	__forceinline Name* createName(const std::string &name)
	{
		Name* n = new Name();
		lookup[name] = n;
		return n;

	}
	__forceinline Name* createIndexForName(const std::string &name,int index)
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

	void reset()
	{
		for(auto it=lookup.begin();it!=lookup.end();it++)
		{
			it->second->clear();
		}

		for(int i=0;i<usedIndexes.size();i++)
		{
			
			if(namePool.size() < POOL_SIZE)
			{
				namePool.push(indexTable[usedIndexes[i]]);
			}
			else
			{
				delete indexTable[usedIndexes[i]];
			}
			indexTable[usedIndexes[i]] = nullptr;
		}

		lookup.clear();
		usedIndexes.clear();
	}
private:
	std::map<std::string,Name*> lookup;
	std::vector<Name*> indexTable;
	std::vector<int> usedIndexes;
	FastStack<Name*> namePool;
};