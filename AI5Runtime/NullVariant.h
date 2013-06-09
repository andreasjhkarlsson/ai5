#pragma once
#include "variant.h"
class NullVariant :
	public Variant
{
public:
	static NullVariant Instance;
	~NullVariant(void);
	std::wostream& format(std::wostream& stream);
	virtual double toFloating();
	virtual __int64 toInteger64();
	virtual int toInteger32();
	virtual bool toBoolean();
	virtual shared_string toString();
private:
	NullVariant(void);
};

