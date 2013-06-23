#include "HashMapVariant.h"


HashMapVariant::HashMapVariant(void): Variant(HASH_MAP)
{
}


HashMapVariant::~HashMapVariant(void)
{
}


void HashMapVariant::cleanup()
{
	for(auto it = map.begin();it!=map.end();it++)
	{
		it->first->release();
		it->second->release();
	}
	map.clear();
}
void HashMapVariant::set(Variant* key,Variant* value)
{
	// Todo clear old key/value.
	key->addRef();
	value->addRef();
	map[key] = value;
}
Variant* HashMapVariant::get(Variant* key)
{
	Variant* res = map[key];
	if(res == nullptr)
	{
		throw RuntimeError(std::wstring(L"No value found for key: ")+(*key->toString()));
	}
	return res;
}