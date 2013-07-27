#include "Scope.h"
#include "StackMachine.h"
#include "gc.h"

Scope::Scope(): indexTable(128,VariantReference<NameVariant>()),usedIndexes(),enclosingScope(), Variant(TYPE)
{
	usedIndexes.reserve(16);
	InitializeLightWeightMutex(&lock);
}

Scope::~Scope()
{

}

Scope::ScopeGuard::ScopeGuard(Scope* scope): scope(scope)
{
	LockLightWeightMutex(&scope->lock);
}
Scope::ScopeGuard::~ScopeGuard()
{
	UnlockLightWeightMutex(&scope->lock);
}

VariantReference<NameVariant> Scope::createName(const UnicodeString& name)
{
	ScopeGuard guard(this);
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
	ScopeGuard guard(this);
	if (lookup.find(name) == lookup.end())
	{
		lookup[name] = GC::alloc<NameVariant,const VariantReference<>&>(nullptr);
	}

	VariantReference<NameVariant> nameObj = lookup[name];

	addNameToIndex(index,nameObj);

	return nameObj;
}

void Scope::insertName(const UnicodeString& name,int index,const VariantReference<NameVariant>& nameVariant)
{
	ScopeGuard guard(this);
	lookup[name] = nameVariant;

	addNameToIndex(index,nameVariant);

}

void Scope::addNameToIndex(size_t index,const VariantReference<NameVariant>& nameVariant)
{
	// No locking needed as this is a private method that should already be locked.
	if((index) >= indexTable.size())
	{
		indexTable.resize(index+1);
	}
	indexTable[index] = nameVariant;

	usedIndexes.push_back(index);

}

VariantReference<NameVariant> Scope::getNameFromString(const UnicodeString &name)
{
	ScopeGuard guard(this);
	return lookup[name];
}

VariantReference<NameVariant> Scope::getNameFromIndex(int index)
{
	ScopeGuard guard(this);
	VariantReference<NameVariant> result = indexTable[index];
	if(result.empty() && !enclosingScope.empty())
		return enclosingScope->getNameFromIndex(index);
	return result;
}

void Scope::setEnclosingScope(VariantReference<Scope> scope)
{
	ScopeGuard guard(this);
	this->enclosingScope = scope;
}