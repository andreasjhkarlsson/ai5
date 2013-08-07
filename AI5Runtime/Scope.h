#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "Variant.h"
#include "NameVariant.h"
#include "PooledObject.h"
#include "types.h"
#include "gc.h"
#include "platform.h"

class StackMachineThread;

// Keeping this class optimized is VERY important.
class Scope: public Variant
{
private:
	typedef std::shared_ptr<std::lock_guard<std::mutex>> guard_ptr;
public:
	friend class GC;
	static const VARIANT_TYPE TYPE = SCOPE;
	static Scope* Create(const wchar_t*);

	~Scope();

	VariantReference<NameVariant> getNameFromString(const UnicodeString &name);
	VariantReference<NameVariant> getNameFromIndex(int index);
	VariantReference<NameVariant> createName(const UnicodeString &name);
	VariantReference<NameVariant> createIndexForName(const UnicodeString &name,int index);
	void insertName(const UnicodeString& name,int index,const VariantReference<NameVariant>& nameVariant);
	void setEnclosingScope(VariantReference<Scope> scope);
	std::wostream& format(std::wostream& stream) const;

private:
	Scope(const wchar_t* name);
	void addNameToIndex(size_t index,const VariantReference<NameVariant>& nameVariant);

	class ScopeGuard
	{
	public:
		ScopeGuard(Scope*);
		~ScopeGuard();
	private:
		Scope* scope;
	};

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

	LightWeightMutex lock;

	const wchar_t* name;
};