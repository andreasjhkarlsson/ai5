#pragma once
#include "variant.h"
#include "IteratorVariant.h"
#include <unordered_map>
#include "VariantReference.h"

class HashMapVariant :
	public Variant
{
public:
	friend class GC;
	typedef std::unordered_map<VariantReference<>,VariantReference<>,VariantKeyHasher,VariantKeyComparator> VariantMap;
	static const VARIANT_TYPE TYPE = HASH_MAP;
	static HashMapVariant* Create();
	~HashMapVariant(void);
	virtual shared_string toString() const;
	virtual std::wostream& format(std::wostream& stream) const;
	void set(const VariantReference<>& key,VariantReference<>& value);
	const VariantReference<>& get(const VariantReference<>& key);
	virtual VariantReference<IteratorVariant> iterate();
	std::unordered_map<VariantReference<>,VariantReference<>>* getMap();
private:
	HashMapVariant(void);
	VariantMap map;

	class KeyIterator: public IteratorVariant
	{
	public:
		KeyIterator(const VariantReference<HashMapVariant>& map);
		virtual bool hasMore();
		virtual VariantReference<> next();
	private:
		VariantMap::iterator it;
		VariantReference<HashMapVariant> map;
	};

};

