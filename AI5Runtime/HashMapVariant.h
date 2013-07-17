#pragma once
#include "variant.h"
#include "IteratorVariant.h"
#include <unordered_map>
class HashMapVariant :
	public Variant
{
public:
	typedef std::unordered_map<VariantReference<>,VariantReference<>,VariantKeyHasher,VariantKeyComparator> VariantMap;
	static const VARIANT_TYPE TYPE = HASH_MAP;
	HashMapVariant(void);
	~HashMapVariant(void);
	virtual void cleanup();
	virtual shared_string toString() const;
	virtual std::wostream& format(std::wostream& stream) const;
	void set(const VariantReference<>& key,VariantReference<>& value);
	const VariantReference<>& get(const VariantReference<>& key);
	virtual IteratorVariant* iterate();
	std::unordered_map<VariantReference<>,VariantReference<>>* getMap();
private:
	VariantMap map;

	class KeyIterator: public IteratorVariant
	{
	public:
		KeyIterator(HashMapVariant* map);
		virtual void cleanup();
		virtual bool hasMore();
		virtual const VariantReference<>& next();
	private:
		VariantMap::iterator it;
		HashMapVariant* map;
	};

};

