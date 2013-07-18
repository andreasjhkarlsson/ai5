#pragma once
#include "variant.h"
#include <string>
#include "gc.h"
class StringVariant :
	public Variant
{
public:
	friend class GC;
	static const VARIANT_TYPE TYPE = STRING;
	
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
	static StringVariant* Create(shared_string);
private:
	shared_string str;
	StringVariant(shared_string str);
};

