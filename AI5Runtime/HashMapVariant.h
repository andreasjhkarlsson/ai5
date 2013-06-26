#pragma once
#include "variant.h"
#include <unordered_map>
class HashMapVariant :
	public Variant
{
public:
	static const VARIANT_TYPE TYPE = HASH_MAP;
	HashMapVariant(void);
	~HashMapVariant(void);
	virtual void cleanup();
	virtual shared_string toString() const;
	virtual std::wostream& format(std::wostream& stream) const;
	void set(Variant* key,Variant* value);
	Variant* get(Variant* key);
private:
	std::unordered_map<Variant*,Variant*,VariantKeyHasher,VariantKeyComparator> map;
};

