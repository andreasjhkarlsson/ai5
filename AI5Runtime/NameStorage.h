#pragma once

#include <vector>
#include <map>
#include <string>
#include "Variant.h"
#include "FastStack.h"

// Represents a name. Can be builtin, global and local
// at the same time. Handy huh?
class Name
{
public:
	Name(std::string name): builtIn(nullptr),global(nullptr), locals(32), name(name)
	{
	}
	virtual ~Name()
	{
	}
	__forceinline bool hasLocal()
	{
		return locals.size() > 0;
	}
	__forceinline bool hasGlobal()
	{
		return global != nullptr;
	}
	__forceinline bool hasBuiltin()
	{
		return builtIn != nullptr;
	}

	__forceinline void pushLocal(Variant* var)
	{
		var->addRef();
		locals.push(var);
	}

	__forceinline void popLocal()
	{
		locals.pop()->release();
	}

	__forceinline void setGlobal(Variant *var)
	{
		var->addRef();
		if(hasGlobal())
			global->release();
		global = var;
	}

	__forceinline void setBuiltin(Variant *var)
	{
		var->addRef();
		if(hasBuiltin())
			builtIn->release();
		builtIn = var;
	}


	__forceinline void setNearest(Variant* var)
	{
		if(hasLocal())
		{
			var->addRef();
			locals.pop()->release();
			locals.push(var);
		}
		else if(hasGlobal())
		{
			var->addRef();
			global->release();
			global = var;
		}
		else
		{
			// TODO: Raise error!!!
		}
	}

	__forceinline Variant* findNearest()
	{
		if(hasLocal())
			return locals.top();
		if(hasGlobal())
			return global;
		if(hasBuiltin())
			return builtIn;
		return nullptr;
	}
private:
	Variant* builtIn;
	Variant* global;
	FastStack<Variant*> locals;
	std::string name;
};



class NameStorage
{
public:
	NameStorage()
	{

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
		Name* n = new Name(name);
		lookup[name] = n;
		return n;

	}
	__forceinline Name* createIndexForName(const std::string &name,int index)
	{
		if (lookup.find(name) == lookup.end())
		{
			lookup[name] = new Name(name);
		}

		if((index) >= indexTable.size())
		{
			indexTable.resize(index+1);
		}

		Name* nameObj = lookup[name];

		indexTable[index] = nameObj;
		return nameObj;
	}
private:
	std::map<std::string,Name*> lookup;
	std::vector<Name*> indexTable;

};