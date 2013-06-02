#include "Scope.h"
#include "StackMachine.h"

NameVariant* Scope::createName(StackMachine* machine,const std::wstring &name)
{
	NameVariant* n = NameVariant::createFromFactory(machine->getVariantFactory());
	lookup[name] = n;
	return n;
}

NameVariant* Scope::createIndexForName(StackMachine* machine,const std::wstring &name,int index)
{
	if (lookup.find(name) == lookup.end())
	{
		lookup[name] = NameVariant::createFromFactory(machine->getVariantFactory());
	}

	NameVariant* nameObj = lookup[name];

	addNameToIndex(index,nameObj);

	return nameObj;
}

void Scope::reset()
{
	// Clear all names in the lookup. Will release variants. 
	for(auto it=lookup.begin();it!=lookup.end();it++)
	{
		it->second->release();
	}

	// Make sure the indexTable is all null's.
	for(int i=0;i<usedIndexes.size();i++)
	{
		indexTable[usedIndexes[i]] = nullptr;
	}

	lookup.clear();
	usedIndexes.clear();
}

void Scope::insertName(const std::wstring& name,int index,NameVariant* nameVariant)
{
	nameVariant->addRef();
	lookup[name] = nameVariant;

	addNameToIndex(index,nameVariant);

}

void Scope::addNameToIndex(int index,NameVariant* nameVariant)
{
	if((index) >= indexTable.size())
	{
		indexTable.resize(index+1);
	}
	indexTable[index] = nameVariant;

	usedIndexes.push_back(index);

}