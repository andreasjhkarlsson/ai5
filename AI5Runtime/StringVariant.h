#pragma once
#include "variant.h"
#include <string>
class StringVariant :
	public Variant
{
public:
	StringVariant(shared_string str);
	StringVariant(const std::wstring& str);
	~StringVariant(void);
	std::wostream& format(std::wostream& stream);
	virtual double toFloating();
	virtual __int64 toInteger64();
	virtual int toInteger32();
	virtual bool toBoolean();
	virtual shared_string toString();
	virtual bool equal(Variant*);
	virtual size_t hash();
private:
	shared_string str;
};

