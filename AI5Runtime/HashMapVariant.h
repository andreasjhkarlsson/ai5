#pragma once
#include "variant.h"
#include <unordered_map>
class HashMapVariant :
	public Variant
{
public:
	HashMapVariant(void);
	~HashMapVariant(void);
	virtual void cleanup();
	void set(Variant* key,Variant* value);
	Variant* get(Variant* key);
private:
	std::unordered_map<Variant*,Variant*,VariantKeyHasher,VariantKeyComparator> map;
};

