#include "HashMapVariant.h"
#include "VariantReference.h"
#include "gc.h"

HashMapVariant::HashMapVariant(void): Variant(TYPE)
{
}


HashMapVariant::~HashMapVariant(void)
{
}

HashMapVariant* HashMapVariant::Create()
{
	return GC::alloc<HashMapVariant>();
}

void HashMapVariant::set(const VariantReference<>& key,VariantReference<>& value)
{
	map[key] = value;
	GC::persistReference(this,value);
}
const VariantReference<>& HashMapVariant::get(const VariantReference<>& key)
{
	const VariantReference<>& res = map[key];

	if(res.empty())
	{
		throw RuntimeError(UnicodeString(L"No value found for key: ")+(*key->toString()));
	}

	return res;
}

shared_string HashMapVariant::toString() const
{
	UnicodeString* str = new UnicodeString();
	(*str) += L"{";

	bool first = true;
	for(auto it=map.begin();it!=map.end();it++)
	{
		if(!first)
			(*str)+=L", ";
		(*str) += *it->first.toString() + L": "+*it->second.toString();


		first = false;
	}

	(*str)+=L"}";

	return shared_string(str);

}
std::wostream& HashMapVariant::format(std::wostream& stream) const
{
	stream << L"HashMapVariant: " << toString()->getTerminatedBuffer();
	return stream;
}

VariantReference<IteratorVariant> HashMapVariant::iterate()
{
	return new KeyIterator(this);
}

HashMapVariant::KeyIterator::KeyIterator(const VariantReference<HashMapVariant>& map):map(map)
{
	GC::persistReference(this,map);
	it = this->map->map.begin();
}

bool HashMapVariant::KeyIterator::hasMore()
{
	return it != map->map.end();	
}
VariantReference<> HashMapVariant::KeyIterator::next()
{
	return (it++)->first;
}

bool HashMapVariant::toBoolean() const
{
	return this->map.size() > 0;
}