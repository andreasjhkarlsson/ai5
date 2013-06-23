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
	virtual shared_string toString();
	virtual std::wostream& format(std::wostream& stream);
	void set(Variant* key,Variant* value);
	Variant* get(Variant* key);
private:
	std::unordered_map<Variant*,Variant*,VariantKeyHasher,VariantKeyComparator> map;
};

