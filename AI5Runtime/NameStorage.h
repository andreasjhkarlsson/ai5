#pragma once

#include <vector>
#include "Variant.h"
#include "FastStack.h"

// Represents a name. Can be builtin, global and local
// at the same time. Handy huh?
class Name
{
public:
	Name(): builtIn(nullptr),global(nullptr), locals(256)
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
};

class NameStorage
{
public:
	NameStorage(int tableSize);
	~NameStorage(void);
	__forceinline Name* getName(int index);
private:
	std::vector<Name*> nameTable;
};

Name* NameStorage::getName(int index)
{
	if(nameTable[index] == nullptr)
	{
		nameTable[index] = new Name();
	}
	return nameTable[index];
}
