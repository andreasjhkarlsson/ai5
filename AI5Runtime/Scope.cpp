#include "Scope.h"
#include "StackMachine.h"
#include "gc.h"

VariantReference<NameVariant> Scope::createName(const UnicodeString& name)
{
	VariantReference<NameVariant> n = GC::alloc<NameVariant,const VariantReference<>&>(nullptr);
	lookup[name] = n;
	return n;
}


Scope* Scope::Create()
{
	return GC::alloc<Scope>();
}

VariantReference<NameVariant> Scope::createIndexForName(const UnicodeString& name,int index)
{
	if (lookup.find(name) == lookup.end())
	{
		lookup[name] = GC::alloc<NameVariant,const VariantReference<>&>(nullptr);
	}

	VariantReference<NameVariant> nameObj = lookup[name];

	addNameToIndex(index,nameObj);

	return nameObj;
}

void Scope::insertName(const UnicodeString& name,int index,VariantReference<NameVariant> nameVariant)
{
	lookup[name] = nameVariant;

	addNameToIndex(index,nameVariant);

}

void Scope::addNameToIndex(size_t index,const VariantReference<NameVariant>& nameVariant)
{
	if((index) >= indexTable.size())
	{
		indexTable.resize(index+1);
	}
	indexTable[index] = nameVariant;

	usedIndexes.push_back(index);

}
