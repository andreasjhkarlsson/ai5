#pragma once
#include "variant.h"
#include <string>
class StringVariant :
	public Variant
{
public:
	static const VARIANT_TYPE TYPE = STRING;
	StringVariant(shared_string str);
	StringVariant(const UnicodeString& str);
	~StringVariant(void);
	std::wostream& format(std::wostream& stream) const;
	virtual double toFloating() const;
	virtual __int64 toInteger64() const;
	virtual int toInteger32() const;
	virtual bool toBoolean() const;
	virtual shared_string toString() const;
	virtual bool equal(Variant*);
	virtual size_t hash() const;
private:
	shared_string str;
};

