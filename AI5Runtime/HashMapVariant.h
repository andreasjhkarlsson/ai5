#pragma once
#include "variant.h"
#include "IteratorVariant.h"
#include <unordered_map>
class HashMapVariant :
	public Variant
{
public:
	typedef std::unordered_map<Variant*,Variant*,VariantKeyHasher,VariantKeyComparator> VariantMap;
	static const VARIANT_TYPE TYPE = HASH_MAP;
	HashMapVariant(void);
	~HashMapVariant(void);
	virtual void cleanup();
	virtual shared_string toString() const;
	virtual std::wostream& format(std::wostream& stream) const;
	void set(Variant* key,Variant* value);
	Variant* get(Variant* key);
	virtual IteratorVariant* iterate();
	std::unordered_map<Variant*,Variant*>* getMap();
private:
	 VariantMap map;



	class KeyIterator: public IteratorVariant
	{
	public:
		KeyIterator(HashMapVariant* map);
		virtual void cleanup();
		virtual bool hasMore();
		virtual Variant* next();
	private:
		VariantMap::iterator it;
		HashMapVariant* map;
	};

};

