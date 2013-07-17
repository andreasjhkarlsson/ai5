#include "HashMapVariant.h"
#include "VariantReference.h"

HashMapVariant::HashMapVariant(void): Variant(TYPE)
{
}


HashMapVariant::~HashMapVariant(void)
{
}


void HashMapVariant::cleanup()
{
	map.clear();
}
void HashMapVariant::set(const VariantReference<>& key,VariantReference<>& value)
{
	// Todo clear old key/value.

	map[key] = value;
}
const VariantReference<>& HashMapVariant::get(const VariantReference<>& key)
{
	const VariantReference<>& res = map[key];
	/* TODO: How to check if invalid key with non-pod type???
	if(res == nullptr)
	{
		throw RuntimeError(UnicodeString(L"No value found for key: ")+(*key->toString()));
	}
	*/
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

IteratorVariant* HashMapVariant::iterate()
{
	return new KeyIterator(this);
}

HashMapVariant::KeyIterator::KeyIterator(HashMapVariant* map):map(map)
{
	map->addRef();
	it = map->map.begin();

}
void HashMapVariant::KeyIterator::cleanup()
{
	map->release();
}
bool HashMapVariant::KeyIterator::hasMore()
{
	return it != map->map.end();	
}
const VariantReference<>& HashMapVariant::KeyIterator::next()
{
	return (it++)->first;
}

