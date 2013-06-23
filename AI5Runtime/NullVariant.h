#pragma once
#include "variant.h"
class NullVariant :
	public Variant
{
public:
	static NullVariant Instance;
	~NullVariant(void);
	std::wostream& format(std::wostream& stream) const;
	virtual double toFloating() const;
	virtual __int64 toInteger64() const;
	virtual int toInteger32() const;
	virtual bool toBoolean() const;
	virtual shared_string toString() const;
	virtual bool equal(Variant*);
private:
	NullVariant(void);
};

